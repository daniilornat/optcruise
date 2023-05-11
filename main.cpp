#include "graph.hpp"
#include "search.hpp"
#include "readInput.hpp"
#include "interface.hpp"
#include "logger.hpp"

// вывод пути в виде строки
void display_path( Path &paths,
                   const std::vector<int> &distances,
                   const id &from_city,
                   const id &to_city,
                   const IDStore &city_id_store,
                   const IDStore &transport_id_store ) {

    if ( distances[to_city] == INF ) {

        clear( );
        refresh( );

        attron(COLOR_PAIR(4));
        mvprintw(1, 0, "Искомый город недостижим из города отправления");
        attroff(COLOR_PAIR(4));
        wait_for_enter(2);

        } else {
            const auto path_string = path_to_string( paths,
                                                     from_city,
                                                     to_city,
                                                     city_id_store,
                                                     transport_id_store );

            clear( );
            refresh( );

            printw("Искомый путь: ");
            attron(COLOR_PAIR(4));
            mvprintw(2, 0, "%s", path_string.c_str());
            attroff(COLOR_PAIR(4));
        }
}

// вывод доступных по ограничениям путей
void display_limit_path( Path paths,
                         const std::vector<id> &available_cities,
                         const id &from_city,
                         const IDStore &city_id_store,
                         const IDStore &transport_id_store ) {

    const auto path_string = limit_paths_to_string( paths,
                                                    available_cities,
                                                    from_city,
                                                    city_id_store,
                                                    transport_id_store );

    clear( );
    refresh( );

    attron(COLOR_PAIR(4));
    mvprintw(1, 0, "%s", path_string.c_str());
    attroff(COLOR_PAIR(4));
}

int main( int argc, char **argv ) {
    setlocale(LC_ALL, "");

    float total_time;

    std::vector<Edge> arcs;
    IDStore city_id_store;
    IDStore transport_id_store;

    // считывание данных
    if (argc < 2)
    {
        std::cout << "Введите название файла, содержащего города" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Пожалуйста подождите, идет считывание из файла..." << std::endl;
        read_data( argv[1], arcs, city_id_store, transport_id_store );
    }
    
    Graph graph { arcs, city_id_store.size( ) };

    // кратчайшие пути до городов
    Path shortest_paths { city_id_store.size( ) };

    // ребра через которые проходит кратчайший путь от города отправления до города назначения
    std::vector<Edge> path_arcs;

    // кратчайшие расстояния
    std::vector<int> distances;

    // города достижимые из города отправления с ограничениями
    std::vector<int> available_cities;

    initscr();
    noecho();
    cbreak();
    halfdelay(5);

    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, 10, COLOR_BLACK);
    init_pair(3, 13, COLOR_BLACK);
    init_pair(4, 15, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);

    keypad(stdscr, true);

    bool proccess_in_interaction = true;

    // взаимодействие с пользователем
    while ( proccess_in_interaction ) 
    {
        path_arcs.clear( );
        distances.clear( );
        available_cities.clear( );

        clear();
        refresh();

        // считывание режима работы
        const auto chosen_mode = handle_mode_input( );
        
        clear();
        refresh();

        if ( chosen_mode != -1 ) 
        {
            if ( chosen_mode < 3 )
            {
                // ввод городов для которых нужно найти маршрут
                const auto city_pair = handle_pair_of_cities_input( city_id_store );

                // возобновление работы программы при неправильном вводе названия города
                if (city_pair.first == -1)
                {
                    break;
                }
                else 
                {
                    clear( );
                    refresh( );
                    // ввод используемого транспорта
                    const auto restricted_transport = handle_restricted_transport_list_input(transport_id_store);

                    clear( );
                    refresh( );

                    switch( chosen_mode ) {
                        // минимальные по стоимости среди минимальных по времени
                        case 0:
                        {
                            const auto start_time = std::chrono::high_resolution_clock::now();
                            
                            distances = dijkstra( graph,
                                                  shortest_paths,
                                                  city_pair.first,
                                                  restricted_transport );

                            total_time = estimate_time( start_time.time_since_epoch( ) );

                            display_path( shortest_paths,
                                          distances,
                                          city_pair.first,
                                          city_pair.second,
                                          city_id_store,
                                          transport_id_store );

                            break;
                        }
                        // минимальные по стоимости
                        case 1:
                        {
                            const auto start_time = std::chrono::high_resolution_clock::now();

                            distances = dijkstra( graph,
                                                  shortest_paths,
                                                  city_pair.first,
                                                  restricted_transport,
                                                  true );
                            
                            total_time = estimate_time( start_time.time_since_epoch( ) );

                            display_path( shortest_paths,
                                          distances,
                                          city_pair.first,
                                          city_pair.second,
                                          city_id_store,
                                          transport_id_store );

                            break;
                        }
                        // минимальные по числу посещенных городов
                        case 2:
                        {
                            const auto start_time = std::chrono::high_resolution_clock::now();

                            distances = bfs( graph,
                                             shortest_paths,
                                             city_pair.first,
                                             restricted_transport );

                            total_time = estimate_time( start_time.time_since_epoch( ) );

                            display_path( shortest_paths,
                                          distances,
                                          city_pair.first,
                                          city_pair.second,
                                          city_id_store,
                                          transport_id_store );
                            break;
                        }
                        default:
                            break;
                    } 
                }
            // режимы работы с ограничениями
            } else {

                const auto city_from_id = handle_single_city_input(city_id_store);

                clear();
                refresh();

                long limit;

                if (chosen_mode == 3)
                {
                    limit = handle_cost_limit_input();
                }
                else
                {
                    limit = handle_time_limit_input();
                }

                clear();
                refresh();

                const auto restricted_transports = handle_restricted_transport_list_input(transport_id_store);

                clear();
                refresh();

                switch (chosen_mode)
                {
                // ограничение по стоимости
                case 3:
                {
                    const auto start_time = std::chrono::high_resolution_clock::now();

                    available_cities = dijkstra( graph,
                                                 shortest_paths,
                                                 city_from_id,
                                                 restricted_transports,
                                                 false,
                                                 0,
                                                 limit );

                    

                    total_time = estimate_time( start_time.time_since_epoch( ) );

                    display_limit_path( shortest_paths,
                                        available_cities,
                                        city_from_id,
                                        city_id_store,
                                        transport_id_store );

                    break;
                }
                case 4:
                {
                    const auto start_time = std::chrono::high_resolution_clock::now();

                    available_cities = dijkstra( graph,
                                                 shortest_paths,
                                                 city_from_id,
                                                 restricted_transports,
                                                 false,
                                                 limit,
                                                 0 );

                    total_time = estimate_time( start_time.time_since_epoch( ) );

                    display_limit_path( shortest_paths,
                                        available_cities,
                                        city_from_id,
                                        city_id_store,
                                        transport_id_store );

                    break;
                }
                default:
                    break;
                }
            }
        } else {
            break;
        }

        // завершение работы программы
        char c;

        while ((c = getch()) != 10) {}

        clear();
        refresh();

        const auto result = handle_finish_app();

        if (result == 0)
        {
            proccess_in_interaction = false;
        }
    }

    endwin();

    return 0;
}