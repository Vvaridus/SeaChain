#include <string>
#include <Windows.h>
#include <iostream>
#include <shlobj.h>
#include <SFML/Window/Keyboard.hpp>
#include "convert.h"

std::string Converter::SFKeyToString(sf::Keyboard::Key key) {
    std::string out;

    switch (key) {
    case sf::Keyboard::Key::Num1:
        out = std::string("1"); break;
    case sf::Keyboard::Key::Num2:
        out = std::string("2"); break;
    case sf::Keyboard::Key::Num3:
        out = std::string("3"); break;
    case sf::Keyboard::Key::Num4:
        out = std::string("4"); break;
    case sf::Keyboard::Key::Num5:
        out = std::string("5"); break;
    case sf::Keyboard::Key::Num6:
        out = std::string("6"); break;
    case sf::Keyboard::Key::Num7:
        out = std::string("7"); break;
    case sf::Keyboard::Key::Num8:
        out = std::string("8"); break;
    case sf::Keyboard::Key::Num9:
        out = std::string("9"); break;
    case sf::Keyboard::Key::Num0:
        out = std::string("0"); break;
    case sf::Keyboard::Key::Return:
        out = std::string("Enter"); break;
    case sf::Keyboard::Key::BackSpace:
        out = std::string("BackSpace"); break;
    case sf::Keyboard::Key::Space:
        out = std::string("Space"); break;
    case sf::Keyboard::Key::Left:
        out = std::string("Left"); break;
    case sf::Keyboard::Key::Down:
        out = std::string("Down"); break;
    case sf::Keyboard::Key::Right:
        out = std::string("Right"); break;
    case sf::Keyboard::Key::Up:
        out = std::string("Up"); break;
    case sf::Keyboard::Key::Delete:
        out = std::string("Delete"); break;
    case sf::Keyboard::Key::A:
        out = std::string("A"); break;
    case sf::Keyboard::Key::B:
        out = std::string("B"); break;
    case sf::Keyboard::Key::C:
        out = std::string("C"); break;
    case sf::Keyboard::Key::D:
        out = std::string("D"); break;
    case sf::Keyboard::Key::E:
        out = std::string("E"); break;
    case sf::Keyboard::Key::F:
        out = std::string("F"); break;
    case sf::Keyboard::Key::G:
        out = std::string("G"); break;
    case sf::Keyboard::Key::H:
        out = std::string("H"); break;
    case sf::Keyboard::Key::I:
        out = std::string("I"); break;
    case sf::Keyboard::Key::J:
        out = std::string("J"); break;
    case sf::Keyboard::Key::K:
        out = std::string("K"); break;
    case sf::Keyboard::Key::L:
        out = std::string("L"); break;
    case sf::Keyboard::Key::M:
        out = std::string("M"); break;
    case sf::Keyboard::Key::N:
        out = std::string("N"); break;
    case sf::Keyboard::Key::O:
        out = std::string("O"); break;
    case sf::Keyboard::Key::P:
        out = std::string("P"); break;
    case sf::Keyboard::Key::Q:
        out = std::string("Q"); break;
    case sf::Keyboard::Key::R:
        out = std::string("R"); break;
    case sf::Keyboard::Key::S:
        out = std::string("S"); break;
    case sf::Keyboard::Key::T:
        out = std::string("T"); break;
    case sf::Keyboard::Key::U:
        out = std::string("U"); break;
    case sf::Keyboard::Key::V:
        out = std::string("V"); break;
    case sf::Keyboard::Key::W:
        out = std::string("W"); break;
    case sf::Keyboard::Key::X:
        out = std::string("X"); break;
    case sf::Keyboard::Key::Y:
        out = std::string("Y"); break;
    case sf::Keyboard::Key::Z:
        out = std::string("Z"); break;
    case sf::Keyboard::Key::Tilde:
        out = std::string("~"); break;
    case sf::Keyboard::Key::Tab:
        out = std::string("TAB"); break;
    case sf::Keyboard::Key::LControl:
        out = std::string("L CTRL"); break;
    case sf::Keyboard::Key::LAlt:
        out = std::string("L ALT"); break;
    case sf::Keyboard::Key::RAlt:
        out = std::string("R ALT"); break;
    case sf::Keyboard::Key::RControl:
        out = std::string("R CTRL"); break;
    case sf::Keyboard::Key::SemiColon:
        out = std::string(";"); break;
    case sf::Keyboard::Key::Equal:
        out = std::string("="); break;
    case sf::Keyboard::Key::Comma:
        out = std::string("<"); break;
    case sf::Keyboard::Key::Dash:
        out = std::string("-"); break;
    case sf::Keyboard::Key::Period:
        out = std::string(">"); break;
    case sf::Keyboard::Key::Divide:
        out = std::string("?"); break;
    case sf::Keyboard::Key::LBracket:
        out = std::string("L Bracket"); break;
    case sf::Keyboard::Key::Slash:
        out = std::string("\\"); break;
    case sf::Keyboard::Key::RBracket:
        out = std::string("R Bracket"); break;
    case sf::Keyboard::Key::Quote:
        out = std::string("Quote"); break;
    }
}