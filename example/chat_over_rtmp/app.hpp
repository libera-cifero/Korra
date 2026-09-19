#include <ftxui/ftxui.hpp>
#include <functional>
#include <string>
using namespace std;
using namespace ftxui;
class app {
private:
    string _message, _chat_history_text, _my_name, _my_ip, _interlocutor_ip;
    bool _interlocutor_online = false;
    Component _build_ui();
public:
    app(string my_name, string my_ip, string interlocutor_ip);
    function<void(string text)> on_message_recieved;
    function<void(string text)> on_message_sent;
    function<void(string username)> on_user_connected;
    function<void()> on_user_disconnected;
    void run();
};