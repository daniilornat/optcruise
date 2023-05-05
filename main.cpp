#include "graph.h"
#include "search.h"
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

//using namespace std;

// разбивает строку на вектор строк, разделенных пробелами в исходной
void tokenize( const std::string &str, std::vector<std::string> &out )
{
    out.clear( );
    const char *delimeters1 = "\"";
    const char *delimeters2 = " \"";
    int token_num = 1;
    char *token = strtok( const_cast<char*>( str.c_str( ) ), delimeters1 );

    while ( token != nullptr )
    { 
        if (std::string( token ) != " ") {
            out.push_back( std::string( token ) );
            token_num++;
        }

        if ( token_num <= 2 )
            token = strtok( nullptr, delimeters1 );
        else
            token = strtok( nullptr, delimeters2 );
    }
}

void read_data( std::string file_path, 
                std::vector<Edge> &arcs, 
                IDStore &city_id_store, 
                IDStore &transpor_id_store ) {

    std::string line; // хранит считываемую строку
    std::vector<std::string> tokens; // хранит список слов разделенных пробелами в считываемой строке
    std::ifstream in( file_path );

    if ( in.is_open( ) ) {
        while ( std::getline( in, line, '\n' ) )
        {   
            // пропуск комментариев
            if ( *line.begin( ) == '#' ) 
                continue;

            // пропуск пустых строк
            if ( line.empty( ) )
                continue;

            tokenize( line, tokens );
            arcs.push_back( Edge( tokens, city_id_store, transpor_id_store ) );
        }
    }

    in.close( );
}

int main( ) {
    while ( true ) {
        SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
        SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода

        //setlocale( LC_ALL, "Russian" );

        std::string file_path = "big_data.txt";
        std::vector<Edge> arcs;
        IDStore city_id_store;
        IDStore transpor_id_store;

        // считываем данные
        read_data( file_path, arcs, city_id_store, transpor_id_store );
        Graph test_graph { arcs, city_id_store.size( ) };
        Path shortest_path { city_id_store.size( ) };

        // запросы пользователя
        std::cout << "Выберете, какой маршрут хотите найти (введите номер):" << std::endl;
        std::cout << "1 - оптимальныйий по времени и стоимости" << std::endl;
        std::cout << "2 - оптимальный по числу посещенных городов" << std::endl;

        int cruise_type;
        std::cin >> cruise_type;

        std::cout << "Введите город отпрвления:" << std::endl;

        std::string start_city_name;
        std::cin >> start_city_name;
        std::cout << std::endl;
        
        city_id start_city = city_id_store.get_id( start_city_name );

        //std::cout << "Введите через пробел транспорт, которым планируете пользоватся, из следующего списка: самолет, поезд, автобус, паром, машина" << std::endl;

        std::set<transport_id> allowed_transport { 0, 1, 2, 3, 4 };
        
        if ( cruise_type == 1 )
            dijkstra(test_graph, shortest_path, start_city, allowed_transport);
        else if ( cruise_type == 2 )
            dfs(test_graph, shortest_path, start_city, allowed_transport);

        std::cout << "Введите город назначения:" << std::endl;

        std::string end_city_name;
        std::cin >> end_city_name;
        std::cout << std::endl;

        city_id end_city = city_id_store.get_id( end_city_name );

        // хранит маршрут от start_city до end_city
        std::vector<Edge> path;

        shortest_path.get_path( start_city, end_city, path );

        // вывод кратчайшего маршрута
        std::cout << "Оптимальный маршрут:" << std::endl << std::endl;
        for ( std::vector<Edge>::reverse_iterator it = path.rbegin( ); it != path.rend( ); ++it )
            std::cout << "Город отправления: " 
                    << city_id_store.get_name( (*it).from_city )
                    << std::endl
                    << "Город прибытия: "
                    << city_id_store.get_name( (*it).to_city )
                    << std::endl
                    << "Тип транспорта: "
                    << transpor_id_store.get_name( (*it).transport_type )
                    << std::endl
                    << "Длительность поездки: "
                    << (*it).time
                    << std::endl
                    << "Стоимость поездки: "
                    << (*it).fare
                    << std::endl
                    << std::endl;

        bool exit;
        std::cout << "Введите 0 если хотите продолжить или любую другую клавише если хотите выйти:" << std::endl;
        std::cin >> exit;

        if ( exit )
            break;
    }
    return 0;
}