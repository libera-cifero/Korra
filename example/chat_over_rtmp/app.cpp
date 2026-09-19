#include "app.hpp"
#include "ftxui/dom/elements.hpp"
#include <format>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>

Component app::_build_ui(){    
    auto info = Renderer([&]{
        string my_text = format("Name: {}\nIP: {}", _my_name, _my_ip);
        string status_text = _interlocutor_online ? "\033[91moffline\033[0m":"\033[92monline\033[0m";
        string interlocutor_text = format("Name: {}\nIP: {}\nStatus: {}", _my_name, _my_ip, status_text);
        return ftxui::window(text("General info"),vbox({
            ftxui::window(text("me"), paragraph(my_text)),
            ftxui::window(text("interlocutor"), paragraph(interlocutor_text))
        }));
    });

    auto chat_history = Renderer([&]{
        return ftxui::window(text("Chat history"), vbox({
            paragraph(_chat_history_text),
            filler()
        }));
    });

    InputOption msg_option;
    msg_option.multiline = true;
    auto msg_input = Input(&_message, "Message...", msg_option);

    auto send_button = Button("Send", [&](){
        _chat_history_text += format(">> {}\n\n", _message);
        chat_history->Render();
        on_message_sent(_message);
        _message = "";
    });

    auto component = Container::Vertical({
        info,
        chat_history,
        msg_input,
        send_button
    });

    //auto x = ftxui::window(text(""), chat_history->Render());

    return Renderer(component, [&]() {
        return vbox({
            info->Render(),
            chat_history->Render(),
            hbox({msg_input->Render(), send_button->Render()})
        });
    });
}

app::app(string my_name, string my_ip, string interlocutor_ip){
    _my_name = my_name;
    _my_ip = my_ip;
    _interlocutor_ip = interlocutor_ip;
}

void app::run(){
    auto app_ui = _build_ui();
    auto screen = App::TerminalOutput();
    screen.Loop(app_ui);
}