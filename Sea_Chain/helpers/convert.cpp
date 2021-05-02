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

	return out;
}

sf::Keyboard::Key Converter::UInt32ToSFKey(sf::Uint32 key) {
	sf::Keyboard::Key out = sf::Keyboard::A;

	switch (key) {
	case 44:
		out = sf::Keyboard::Comma; break;
	case 42:
		out = sf::Keyboard::Multiply; break;
	case 60:
		out = sf::Keyboard::Tab; break;
	case 36:
		out = sf::Keyboard::Escape; break;
	case 48:
		out = sf::Keyboard::Num0; break;
	case 49:
		out = sf::Keyboard::Num1; break;
	case 50:
		out = sf::Keyboard::Num2; break;
	case 51:
		out = sf::Keyboard::Num3; break;
	case 52:
		out = sf::Keyboard::Num4; break;
	case 53:
		out = sf::Keyboard::Num5; break;
	case 54:
		out = sf::Keyboard::Num6; break;
	case 55:
		out = sf::Keyboard::Num7; break;
	case 56:
		out = sf::Keyboard::Num8; break;
	case 57:
		out = sf::Keyboard::Num9; break;
	case 97:
		out = sf::Keyboard::A; break;
	case 98:
		out = sf::Keyboard::B; break;
	case 99:
		out = sf::Keyboard::C; break;
	case 100:
		out = sf::Keyboard::D; break;
	case 101:
		out = sf::Keyboard::E; break;
	case 102:
		out = sf::Keyboard::F; break;
	case 103:
		out = sf::Keyboard::G; break;
	case 104:
		out = sf::Keyboard::H; break;
	case 105:
		out = sf::Keyboard::I; break;
	case 106:
		out = sf::Keyboard::J; break;
	case 107:
		out = sf::Keyboard::K; break;
	case 108:
		out = sf::Keyboard::L; break;
	case 109:
		out = sf::Keyboard::M; break;
	case 110:
		out = sf::Keyboard::N; break;
	case 111:
		out = sf::Keyboard::O; break;
	case 112:
		out = sf::Keyboard::P; break;
	case 113:
		out = sf::Keyboard::Q; break;
	case 114:
		out = sf::Keyboard::R; break;
	case 115:
		out = sf::Keyboard::S; break;
	case 116:
		out = sf::Keyboard::T; break;
	case 117:
		out = sf::Keyboard::U; break;
	case 118:
		out = sf::Keyboard::V; break;
	case 119:
		out = sf::Keyboard::W; break;
	case 120:
		out = sf::Keyboard::X; break;
	case 121:
		out = sf::Keyboard::Y; break;
	case 122:
		out = sf::Keyboard::Z; break;
	}

	return out;
}

sf::Keyboard::Key Converter::StringToSFKey(std::string key) {
	sf::Keyboard::Key out = sf::Keyboard::A;

	if (key == ",")
		out = sf::Keyboard::Comma;
	if (key == "*")
		out = sf::Keyboard::Multiply;
	if (key == "TAB")
		out = sf::Keyboard::Tab;
	if (key == "Escape")
		out = sf::Keyboard::Escape;
	if (key == "0")
		out = sf::Keyboard::Num0;
	if (key == "1")
		out = sf::Keyboard::Num1;
	if (key == "2")
		out = sf::Keyboard::Num2;
	if (key == "3")
		out = sf::Keyboard::Num3;
	if (key == "4")
		out = sf::Keyboard::Num4;
	if (key == "5")
		out = sf::Keyboard::Num5;
	if (key == "6")
		out = sf::Keyboard::Num6;
	if (key == "7")
		out = sf::Keyboard::Num7;
	if (key == "8")
		out = sf::Keyboard::Num8;
	if (key == "9")
		out = sf::Keyboard::Num9;
	if (key == "A")
		out = sf::Keyboard::A;
	if (key == "B")
		out = sf::Keyboard::B;
	if (key == "C")
		out = sf::Keyboard::C;
	if (key == "D")
		out = sf::Keyboard::D;
	if (key == "E")
		out = sf::Keyboard::E;
	if (key == "F")
		out = sf::Keyboard::F;
	if (key == "G")
		out = sf::Keyboard::G;
	if (key == "H")
		out = sf::Keyboard::H;
	if (key == "I")
		out = sf::Keyboard::I;
	if (key == "J")
		out = sf::Keyboard::J;
	if (key == "K")
		out = sf::Keyboard::K;
	if (key == "L")
		out = sf::Keyboard::L;
	if (key == "M")
		out = sf::Keyboard::M;
	if (key == "N")
		out = sf::Keyboard::N;
	if (key == "O")
		out = sf::Keyboard::O;
	if (key == "P")
		out = sf::Keyboard::P;
	if (key == "Q")
		out = sf::Keyboard::Q;
	if (key == "R")
		out = sf::Keyboard::R;
	if (key == "S")
		out = sf::Keyboard::S;
	if (key == "T")
		out = sf::Keyboard::T;
	if (key == "U")
		out = sf::Keyboard::U;
	if (key == "V")
		out = sf::Keyboard::V;
	if (key == "W")
		out = sf::Keyboard::W;
	if (key == "X")
		out = sf::Keyboard::X;
	if (key == "Y")
		out = sf::Keyboard::Y;
	if (key == "Z")
		out = sf::Keyboard::Z;
	

	return out;
}