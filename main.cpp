#include <iostream>
#include <time.h>
#include <cstdlib>
#include <string>
#include <cmath>
#include "tinyexpr.h"
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

int a, b, c; // a, b e c sao componentes de expressoes
int dice;
int acertos = 0;
std::string eq = "Carrossel é um minijogo onde serão geradas\nexpressões matemáticas para o usuário responder.\nPressione \"Enter\" para começar:";
std::string entrada_user = "";
char conjuntos[5] = { 'N', 'Z', 'Q', 'I', 'R' };
std::string casos[3] = { " está em Z", "* está em Q", " está em R" };
bool errou = 0;
bool start = 0;

// SFML
sf::RenderWindow janela(sf::VideoMode(640, 360), "Carrossel. Pedro N. 2024");
sf::Font clearSans;
sf::Text equacao;
sf::Text erro_tela;
sf::Text resposta;
sf::RectangleShape campo1(sf::Vector2f(580.f, 120.f));
sf::RectangleShape campo2(sf::Vector2f(200.f, 50.f));

class jogo {
public:
    void fazer_expr() { // faz a expressao e manda para a tela
        switch (dice) {
        case 0: // conjuntos
            a = rand() % 5;
            b = rand() % 3;

            eq = conjuntos[a] + casos[b] + "? 1 para \"Sim\", 0 para \"Nao\": ";
            equacao.setString(eq);
            break;
        case 1: // expressoes
            a = rand() % 10 + 1;
            b = rand() % 10 + 1;
            c = rand() % 10 + 1;

            eq = std::to_string(a) + "+-*"[rand() % 3] + std::to_string(b) + "+-*"[rand() % 3] + std::to_string(c);
            equacao.setString(eq + "= ?: ");
            break;
        case 2: // 1grau
            a = rand() % 5 + 2;
            c = rand() % 5;
            b = a * c;

            eq = std::to_string(a) + "*x" + "+-"[rand() % 2] + std::to_string(b); // eq[3] -> sinal
            equacao.setString(eq + "= 0. x = ?: ");
            break;
        case 3: // exponencial
            a = rand() % 4 + 2;
            b = rand() % 4;
            c = pow(a, b);

            eq = std::to_string(a) + "^x = " + std::to_string(c);
            equacao.setString(eq + ". x = ?: ");
            break;
        }
    }
    void valida_eq() { // checa e ve se a resposta esta correta
        erro_tela.setString("");
        if (entrada_user == "") erro_tela.setString("Nada foi digitado. Tente novamente.");
        else {
            switch (dice) {
            case 0: // conjuntos
                if (entrada_user != "0" && entrada_user != "1") { // colocou algo diferente
                    erro_tela.setString("Você colocou um(s) dígito(s) diferente(s). Tente novamente");
                    std::cout << "Você colocou um dígito diferente";
                }
                else if (((a == 0 || a == 1 && b == 1 || a > 0 && b == 2) || conjuntos[a] == casos[b].back()) ^ stoi(entrada_user)) { // errou
                    erro_tela.setString("Você errou!\nVocê acertou " + std::to_string(acertos - 1) + " perguntas!");
                    acertos = 0;
                }
                else {
                    ++acertos;
                    dice = rand() % 4;
                    fazer_expr();
                }
                break;
            case 1: // expressoes
                if (stoi(entrada_user) != te_interp(eq.data(), 0)) {
                    erro_tela.setString("Você errou! O resultado correto é: " + std::to_string((int)te_interp(eq.data(), 0)) + "\nVocê acertou " + std::to_string(acertos - 1) + " perguntas!");
                    acertos = 0;
                }
                else {
                    ++acertos;
                    dice = rand() % 4;
                    fazer_expr();
                }
                break;
            case 2: // 1grau
                if (stoi(entrada_user) != (c * (eq[3] == '+' ? -1 : 1))) {
                    erro_tela.setString("Você errou! O resultado correto é: " + std::to_string(c * (eq[3] == '+' ? -1 : 1)) + "\nVocê acertou " + std::to_string(acertos - 1) + " perguntas!");
                    acertos = 0;
                }
                else {
                    ++acertos;
                    dice = rand() % 4;
                    fazer_expr();
                }
                break;
            case 3: // exponencial
                if (stoi(entrada_user) != b) {
                    erro_tela.setString("Você errou! O resultado correto é: " + std::to_string(b) + "\nVocê acertou " + std::to_string(acertos-1) + " perguntas!");
                    acertos = 0;
                }
                else {
                    ++acertos;
                    dice = rand() % 4;
                    fazer_expr();
                }
                break;
            }
            entrada_user.erase();
        }
    }
};
int main() {
    srand(time(0));
    // contrucao da janela
    janela.setFramerateLimit(60);

    clearSans.loadFromFile("ClearSans-Regular.ttf");
    equacao.setString(eq);
    equacao.setCharacterSize(20);
    equacao.setFont(clearSans);
    equacao.setFillColor(sf::Color::Black);
    equacao.setOrigin(-40.f, -30.f);

    //erro_tela.setString("Pedro N. 2024");
    erro_tela.setCharacterSize(20);
    erro_tela.setFont(clearSans);
    erro_tela.setFillColor(sf::Color::Black);
    erro_tela.setOrigin(-40.f, -80.f);

    resposta.setString(entrada_user);
    resposta.setCharacterSize(32);
    resposta.setFont(clearSans);
    resposta.setFillColor(sf::Color::Black);
    resposta.setOrigin(-230.f, -205.f);

    campo1.setFillColor(sf::Color::White);
    campo1.setOrigin(-32.f, -25.f);
    campo1.setOutlineThickness(6.f);
    campo1.setOutlineColor(sf::Color(0, 0, 0, 100));

    campo2.setFillColor(sf::Color::White);
    campo2.setOrigin(-225.f, -200.f);
    campo2.setOutlineThickness(6.f);
    campo2.setOutlineColor(sf::Color(0, 0, 0, 100));

    jogo g;
    while (janela.isOpen())
    {
        sf::Event event;
        while (janela.pollEvent(event)) { // introducao
            switch (event.type) {
            case sf::Event::Closed:
                janela.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.scancode == sf::Keyboard::Scan::Enter) {
                    if (!start) {
                        entrada_user.erase();
                        start = 1;
                        acertos = 1;
                        dice = rand() % 4;
                        g.fazer_expr();
                    }
                    else if(start && acertos == 0) janela.close();
                    else {
                        g.valida_eq();
                    }
                }
                break;
            case sf::Event::TextEntered:
                switch (event.text.unicode) {
                case '\b':
                    if (entrada_user.length() > 0) entrada_user.pop_back();
                    break;
                default:
                    if ((event.text.unicode == 45 || event.text.unicode > 47 && event.text.unicode < 58) && entrada_user.length() < 10) {
                        entrada_user += event.text.unicode;
                    }
                }
            }
            resposta.setString(entrada_user);
        }
        janela.clear(sf::Color(39, 153, 245, 10));
        janela.draw(campo1);
        janela.draw(campo2);
        janela.draw(equacao);
        janela.draw(erro_tela);
        janela.draw(resposta);
        janela.display();

    }
    std::cin.ignore();
    std::cin.get();
}