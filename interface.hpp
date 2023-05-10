#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <set>
//#include <sys/resource.h>
#include <ncurses.h>
#include <locale>
#include <chrono>
#include <iomanip>
#include <ctime>

#include "graph.hpp"

#define ctrl(x) (x & 0x1F)


const std::string EXIT_APP = "EXIT_APP";

// очищет строки терминала с start по end
void clear_n_lines(int start, int end)
{
    for (int i = start; i <= end; i++)
    {
        move(i, 0);
        clrtoeol();
    }
}

// ожидание ввода клавиши ENTER
void wait_for_enter(int start)
{

    curs_set(0);
    attron(COLOR_PAIR(3));
    mvprintw(start, 0, "Нажмите ENTER чтобы продолжить");
    attroff(COLOR_PAIR(3));
    move(start + 1, 0);
    char c;
    while ((c = getch()) != 10) {}
}

// оценка времени работы программы
float estimate_time(const std::chrono::steady_clock::duration &start_time_duration)
{
    const auto end_time = std::chrono::high_resolution_clock::now();
    const auto time = end_time.time_since_epoch() - start_time_duration;
    return std::chrono::duration<float>(time).count();
}

// проверка корректности ввода числа
int validate_number_input(const std::string &input) {
    for (int i = 0; i < input.length(); i++)
        if (std::isdigit(input[i]) == false)
            return -1;

    return stoi(input);
}

// проверка корректности ввода имен городов
id validate_city_input(const IDStore &city_id_store, const std::string &input)
{
    for (const auto [city_name, city_id] : city_id_store.titles_to_ids)
        if (city_name == input)
            return city_id_store.get( city_name );  
    
    return -1;
}

// проверка корректности ввода транспорта
id validate_transport_input(const IDStore &transpor_id_store, const std::string &input)
{
    for (const auto [transport_name, transport_id] : transpor_id_store.titles_to_ids)
        if (transport_name == input)
            return transpor_id_store.get( transport_name );

    return -1;
}

// обработка ввода режима работы
// выводит на экран доступные режимы
// считывает и возвращает выбор пользователя
int handle_mode_input()
{
    curs_set(0);
    int chosen_mode = 0;
    const unsigned int modes_count = 6;
    const std::string modes[modes_count] = {
        " 1. Среди кратчайших по времени путей между двумя городами найти путь минимальной стоимости",
        " 2. Среди путей между двумя городами найти путь минимальной стоимости",
        " 3. Среди путей между двумя городами найти путь минимальный по числу посещенных городов",
        " 4. Найти множество городов (и минимальных по стоимости путей к ним), достижимых из города отправления\n     не более чем за обозначенную сумму денег",
        " 5. Найти множество городов (и минимальных по времени путей к ним), достижимых из города отправления\n     не более чем за обозначенное время",
        "Завершить программу"};
    int user_choice;
    bool is_proccessing = true;

    mvprintw(0, 0, "Добро пожаловать! Выберите режим работы");

    while (is_proccessing) {

        for (int i = 0; i < modes_count; i++) {
            if (chosen_mode == i) {
                attron(A_REVERSE);
            } 
            
            if (i == 4) {
                mvprintw(i + 3, 0, "%s", modes[i].c_str());
            } else if (i == 5) {
                attron(COLOR_PAIR(5));
                mvprintw(i + 5, 0, "%s", modes[i].c_str());
                attroff(COLOR_PAIR(5));
            } else {
                mvprintw(i + 2, 0, "%s", modes[i].c_str());
            }

            attroff(A_REVERSE);
        }
        user_choice = getch();

        switch (user_choice)
        {
        case KEY_UP:
            if (chosen_mode == 0)
            {
                chosen_mode = modes_count - 1;
            }
            else
            {
                chosen_mode--;
            }
            break;
        case KEY_DOWN:
            if (chosen_mode == modes_count - 1)
            {
                chosen_mode = 0;
            }
            else
            {
                chosen_mode++;
            }
            break;
        default:
            break;
        }

        if (user_choice == 10) {
            is_proccessing = false;
            if (chosen_mode == 5) {
                return -1;
            }
            else {
                return chosen_mode;
            }
        }
    }
    return -1;
}

// обработка завершения работы пользователем
int handle_finish_app()
{
    curs_set(0);
    const unsigned int modes_count = 2;
    int chosen_mode = 0;
    const std::string modes[modes_count] = {
        " 1. Завершить программу",
        " 2. Вернутся на страницу выбора режима работы"};
    int user_choice;
    bool is_proccessing = true;

    mvprintw(0, 0, "Выберите режим работы");

    while (is_proccessing) {

        for (int i = 0; i < modes_count; i++) {
            if (chosen_mode == i)
                attron(A_REVERSE);

            mvprintw(i + 2, 0, "%s", modes[i].c_str());
            attroff(A_REVERSE);
        }

        user_choice = getch();

        switch (user_choice) {

        case KEY_UP:
            if (chosen_mode == 0) {
                chosen_mode = modes_count - 1;
            } else {
                chosen_mode--;
            }
            break;

        case KEY_DOWN:
            if (chosen_mode == modes_count - 1)  {
                chosen_mode = 0;
            } else {
                chosen_mode++;
            }
            break;

        default:
            break;
        }

        if (user_choice == 10)
            return chosen_mode;
    }

    return chosen_mode;
}

// обработка ввода числа
std::string handle_int_input()
{
    echo();
    char input[200];
    attron(COLOR_PAIR(4));
    getnstr(input, 200);
    attroff(COLOR_PAIR(4));
    std::string str(input);
    noecho();
    return str;
}

// Обработка ввода строки
std::string handle_string_input(int row)
{
    int c;
    std::string str = "";
    std::string new_typed = "";
    int i = 0;
    bool backspace_tap_after_input = true;
    while ((c = getch()) != 10)
    {
        if (c != -1 && (c < 258 || c > 261))
        {
            if (c == 127 || c == KEY_BACKSPACE)
            {
                if (backspace_tap_after_input)
                {
                    i -= ((new_typed.length() / 2));
                    backspace_tap_after_input = false;
                    new_typed.clear();
                }

                if (i > 0)
                {
                    if (str.back() < '0')
                    {
                        str.pop_back();
                        str.pop_back();
                    }
                    else
                    {
                        str.pop_back();
                    }
                    i -= 1;
                    move(row, i);
                }
                else
                {
                    i = 0;
                }
                clrtoeol();
            }
            else if (c == ctrl(27))
            {
                return EXIT_APP;
            }
            else if (char(c) < '0')
            {
                backspace_tap_after_input = true;
                attron(COLOR_PAIR(4));
                printw("%c", c);
                attroff(COLOR_PAIR(4));
                str.push_back(char(c));
                new_typed.push_back(char(c));
                i++;
            }
            else
            {
                backspace_tap_after_input = true;
                attron(COLOR_PAIR(4));
                printw("%c", c);
                attroff(COLOR_PAIR(4));
                str.push_back(char(c));
                new_typed.push_back(char(c));
                new_typed.push_back(char(c));
                i += 2;
            }
        }
    }
    return str;
}

// обработка ввода города отправления и города назначения
std::pair<id, id> handle_pair_of_cities_input(IDStore &city_id_store)
{
    curs_set(1);
    printw("Введите город отправления");

    move(1, 0);

    refresh();

    std::string city_from;
    int city_from_id = -1;
    city_from = handle_string_input(1);

    if (city_from == EXIT_APP)
    {
        return std::make_pair(-1, -1);
    }

    while (city_from_id == -1)
    {
        city_from_id = validate_city_input(city_id_store, city_from);

        if (city_from_id == -1)
        {
            clear_n_lines(3, 4);
            mvprintw(3, 0, "Введите корректный город отправления");
            move(4, 0);
            city_from = handle_string_input(4);

            if (city_from == EXIT_APP)
            {
                return std::make_pair(-1, -1);
            }
        }
    }

    clear_n_lines(0, 4);

    attron(COLOR_PAIR(3));
    mvprintw(0, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));
    mvprintw(2, 0, "Ваш город отправления: ");
    attron(COLOR_PAIR(2));
    mvprintw(2, 23, "%s", city_from.c_str());
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(3));
    mvprintw(4, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));

    move(6, 0);

    printw("Введите город прибытия");

    move(7, 0);

    refresh();

    std::string city_to;
    id city_to_id = -1;
    city_to = handle_string_input(7);

    while (city_to_id == -1)
    {
        city_to_id = validate_city_input(city_id_store, city_to);

        if (city_to_id == -1)
        {
            clear_n_lines(9, 10);
            mvprintw(9, 0, "Введите корректный город прибытия");
            move(10, 0);
            city_to = handle_string_input(10);
            if (city_to == EXIT_APP)
            {
                return std::make_pair(-1, -1);
            }
        }
    }

    clear_n_lines(6, 10);

    attron(COLOR_PAIR(3));
    mvprintw(6, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));
    mvprintw(8, 0, "Ваш город прибытия: ");
    attron(COLOR_PAIR(2));
    mvprintw(8, 20, "%s", city_to.c_str());
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(3));
    mvprintw(10, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));

    wait_for_enter(12);

    return std::make_pair(city_from_id, city_to_id);
}

// обработка ввода города отправления (для режима с ограничениями по времени и стоимости)
id handle_single_city_input(IDStore &city_id_store)
{
    curs_set(1);
    printw("Введите город отправления");

    move(1, 0);

    refresh();

    std::string city_from;
    id city_from_id = -1;
    city_from = handle_string_input(1);

    while (city_from_id == -1)
    {
        city_from_id = validate_city_input(city_id_store, city_from);

        if (city_from_id == -1)
        {
            clear_n_lines(3, 4);
            mvprintw(3, 0, "Введите корректный город отправления");
            move(4, 0);
            city_from = handle_string_input(4);

            if (city_from == EXIT_APP)
            {
                return -1;
            }
        }
    }

    clear_n_lines(0, 4);

    attron(COLOR_PAIR(3));
    mvprintw(0, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));
    mvprintw(2, 0, "Ваш город отправления: ");
    attron(COLOR_PAIR(2));
    mvprintw(2, 23, "%s", city_from.c_str());
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(3));
    mvprintw(4, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));

    wait_for_enter(6);

    return city_from_id;
}

// обработка ограничений по стоимости
long handle_cost_limit_input()
{
    curs_set(1);
    printw("Введите ограничение по стоимости");

    move(1, 0);

    refresh();

    std::string cost_limit_input;
    long cost_limit = -1;
    cost_limit_input = handle_int_input();

    while (cost_limit == -1)
    {
        cost_limit = validate_number_input(cost_limit_input);

        if (cost_limit == -1)
        {
            clear_n_lines(3, 4);
            mvprintw(3, 0, "Число отрицательно или введено некорректно, повторите попытку");
            move(4, 0);
            cost_limit_input = handle_int_input();

            if (cost_limit_input == EXIT_APP)
            {
                return -1;
            }
        }
    }

    clear_n_lines(0, 4);

    attron(COLOR_PAIR(3));
    mvprintw(0, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));
    mvprintw(2, 0, "%s", "Ограничение по стоимости: ");
    attron(COLOR_PAIR(2));
    mvprintw(2, 26, "%s", cost_limit_input.c_str());
    mvprintw(2, 27 + cost_limit_input.length(), "рублей");
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(3));
    mvprintw(4, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));

    wait_for_enter(6);

    return cost_limit;
}

// обработка ввода ограничений по времени
long handle_time_limit_input()
{
    curs_set(1);
    printw("Введите ограничение по времени");

    move(1, 0);

    refresh();

    std::string time_limit_input;
    long time_limit = -1;
    time_limit_input = handle_int_input();

    while (time_limit == -1)
    {
        time_limit = validate_number_input(time_limit_input);
        if (time_limit == -1)
        {
            clear_n_lines(3, 4);
            mvprintw(3, 0, "Число отрицательно или введено некорректно, повторите попытку");
            move(4, 0);
            time_limit_input = handle_int_input();
            if (time_limit_input == EXIT_APP)
            {
                return -1;
            }
        }
    }

    clear_n_lines(0, 4);

    attron(COLOR_PAIR(3));
    mvprintw(0, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));
    mvprintw(2, 0, "%s", "Ограничение по времени: ");
    attron(COLOR_PAIR(2));
    mvprintw(2, 24, "%s", time_limit_input.c_str());
    mvprintw(2, 25 + time_limit_input.length(), "у. е.");
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(3));
    mvprintw(4, 0, "######//~~~~~~//######");
    attroff(COLOR_PAIR(3));

    wait_for_enter(6);

    return time_limit;
}

// обработка ввода видов транспорта доступных для использования
std::set<id> handle_restricted_transport_list_input(IDStore &transport_id_store)
{
    curs_set(1);
    printw("Введите список транспорта, которым не планируете пользоватся, в таком формате (автобус/самолет/.../)");

    bool input_is_valid = false;

    int row_number = 0;
    std::set<id> valid_transport_id_list;
    while (!input_is_valid)
    {

        move(row_number + 1, 0);
        clrtoeol();

        refresh();

        std::set<std::string> invalid_transport_title_list;
        valid_transport_id_list.clear();
        std::string current_transport;

        std::string transport_list_str = handle_string_input(row_number + 1);

        for (const auto &c : transport_list_str)
        {
            if (c == '/')
            {
                const auto valid_transport_id = validate_transport_input(transport_id_store, current_transport);
                if (valid_transport_id != -1)
                {
                    valid_transport_id_list.insert(valid_transport_id);
                }
                else
                {
                    invalid_transport_title_list.insert(current_transport);
                }
                current_transport.clear();
            }
            else
            {
                current_transport.push_back(c);
            }
        }

        if (!invalid_transport_title_list.empty())
        {
            move(4, 0);
            clrtoeol();
            move(6, 0);
            clrtoeol();

            mvprintw(4, 0, "%s", "Транспорт введен некорректно или не найден");

            std::string invalid_input;
            for (const auto &item : invalid_transport_title_list)
            {
                invalid_input += item + " ";
            }

            attron(COLOR_PAIR(4));
            mvprintw(6, 0, "%s", invalid_input.c_str());
            attroff(COLOR_PAIR(4));

            mvprintw(8, 0, "Введите список снова");
            row_number = 8;
        }
        else
        {
            clear_n_lines(0, 9);

            if (valid_transport_id_list.empty())
            {
                mvprintw(0, 0, "Ограничений на транспорт нет");
                wait_for_enter(2);
            }
            else
            {
                attron(COLOR_PAIR(3));
                mvprintw(0, 0, "######//~~~~~~//######");
                attroff(COLOR_PAIR(3));
                mvprintw(2, 0, "Следующий транспорт ограничен");

                std::string valid_input;
                for (const auto &transport_id : valid_transport_id_list)
                {
                    valid_input += transport_id_store.get( transport_id ) + " ";
                }

                attron(COLOR_PAIR(5));
                mvprintw(4, 0, "%s", valid_input.c_str());
                attroff(COLOR_PAIR(5));
                attron(COLOR_PAIR(3));
                mvprintw(6, 0, "######//~~~~~~//######");
                attroff(COLOR_PAIR(3));
                wait_for_enter(8);
            }
            input_is_valid = true;
        }
    }

    return valid_transport_id_list;
}

#endif