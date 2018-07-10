//-----------------------------------------------------------------------------
// Observable mixin.
//-----------------------------------------------------------------------------

#include <tuple>
#include <utility>

#include <boost/signals2.hpp>


// Convenience wrapper for boost::signals2::signal.
template<typename Signature> class Observer {
public:
    Observer(const Observer&) = delete;
    Observer& operator=(const Observer&) = delete;
    Observer() = default;

private:
    template<typename Observers> friend class Observable;

    using Signal = boost::signals2::signal<Signature>;
    using SignalResult = typename Signal::result_type;

    Signal signal_;
};


// Anything that wishes to be observed must extend this, specifying the event types as per the
// example of WindowObservers below.
template<typename Observers> class Observable {
private:
    using ObserverTable = typename Observers::ObserverTable;

public:
    // Registers an observer. The F&& here is a lambda function that calls a corresponding method on an observer.
    template<size_t ObserverId, typename F>
    boost::signals2::connection
    Register(F&& f) {
        return std::get<ObserverId>(signals_).signal_.connect(std::forward<F>(f));
    }

protected:
    Observable() = default;

    // Notifies observers.
    // Returns ONLY the result of the last slot called, wrapped in boost::optional.
    // This is the default behaviour of boost::signals2::signal, which has a Combiner
    // type defined by default as: Combiner = boost::signals2::optional_last_value<R>.
    template<size_t ObserverId, typename... Args>
    typename std::tuple_element<ObserverId, ObserverTable>::type::SignalResult
    Notify(Args&&... args) const {
        return std::get<ObserverId>(signals_).signal_(std::forward<Args>(args)...);
    }

private:
    ObserverTable signals_;
};


//-----------------------------------------------------------------------------
// Example usage.
//-----------------------------------------------------------------------------

#include <iostream>


// Defines observers for Windows class.
struct WindowObservers {
    enum { ShowEvent, CloseEvent };
    using ObserverTable = std::tuple<
            Observer<void()>,                 // ShowEvent
            Observer<bool(bool force_close)>  // CloseEvent
    >;
};


// Window: our Observable.
class Window: public Observable<WindowObservers> {
public:
    void Show() {
        std::cout << "Window::Show called." << std::endl;
        Notify<WindowObservers::ShowEvent>();
        std::cout << "Window::Show handled." << std::endl << std::endl;
    }

    bool Close(bool force_close = false) {
        std::cout << "Window::Close called: force_close == "
                  << std::boolalpha << force_close << "." << std::endl;

//        const boost::optional<bool> can_close{
//                Notify<WindowObservers::CloseEvent>(force_close) };
        // I don't understand how this is optional<bool> given there are two Observers..
        const auto can_close {
                Notify<WindowObservers::CloseEvent>(force_close) };
        std::cout << "Window::Close handled. can_close == "
                  << std::boolalpha << (!can_close || *can_close) << "."
                  << std::endl << std::endl;

        const bool closing{ force_close || !can_close || *can_close };
        if (closing) {
            // Actually close the window.
            // ...
        }
        return closing;
    }
};

// Application: our Observer.
// Makes connections to the window passed in, and then handles them.
// Disconnects upon destruction.
class Application {
public:
    explicit Application(Window& window, const std::string n) : window_(window), name{n} {
        // Register window observers.
        showConnection = window_.Register<WindowObservers::ShowEvent>([this]() {
            OnWindowShow();
        });

        closeConnection = window.Register<WindowObservers::CloseEvent>([this](bool force_close) {
            return OnWindowClose(force_close);
        });
    }

    ~Application() {
        showConnection.disconnect();
        closeConnection.disconnect();
    }

private:
    void OnWindowShow() {
        std::cout << name << "::OnWindowShow called." << std::endl;
    }

    bool OnWindowClose(bool force_close) {
        std::cout << name << "::WindowClose called: force_close == "
                  << std::boolalpha << force_close << "." << std::endl;
        return force_close;
    }

    boost::signals2::connection showConnection;
    boost::signals2::connection closeConnection;
    const std::string name;
    Window& window_;
};

// This application doesn't give a rat's ass about closing windows, and only cares about showing them.
class IndifferentApplication {
public:
    explicit IndifferentApplication(Window& window, const std::string n) : window_(window), name{n} {
        // Register window observers.
        showConnection = window_.Register<WindowObservers::ShowEvent>([this]() {
            OnWindowShow();
        });
    }

    ~IndifferentApplication() {
        showConnection.disconnect();
    }

private:
    void OnWindowShow() {
        std::cout << name << "::OnWindowShow called." << std::endl;
    }

    boost::signals2::connection showConnection;
    const std::string name;
    Window& window_;
};

// This application lives only to veto attempts to close the window.
class CloseVetoer {
public:
    explicit CloseVetoer(Window& window, const std::string n) : window_(window), name{n} {
        // Register window observers.
        closeConnection = window_.Register<WindowObservers::CloseEvent>([this](bool force_close) {
            std::cout << name << " says NO." << std::endl;
            return false;
        });
    }

    ~CloseVetoer() {
        closeConnection.disconnect();
    }

private:
    boost::signals2::connection closeConnection;
    const std::string name;
    Window& window_;
};

int main() {
    Window window;
    // CloseVetoer can only work if it is at the end of the list, as only the last value is returned,
    CloseVetoer veto{ window, "Vetoer"};
    Application application{ window, "App1" };
    Application application2{ window, "App2" };
    IndifferentApplication application3{ window, "AppIndifferent" };

    // Notify observers.
    window.Show();
    //...
    window.Close(false);
    window.Close(true);
}