import sys
import curses as cs

from ReadData import read_data
from Interface import Display
from Graph import *
from Search import *
from Logger import *

def launch(stdscr):
    finish_res = ""

    if len(sys.argv) < 2:
        return 'Введите название файла, содержащего города'

    else:
        print('Пожалуйста подождите, идет считывание из файла...')
        graph, city_ids_store, transport_ids_store = read_data(sys.argv[1]) #возможно проблема в графе при считывании

    display = Display(stdscr, city_ids_store, transport_ids_store)

    proccess_in_interaction = True

    while proccess_in_interaction:
        chosen_mode = display.handle_mode_input( )

        display.clear( )
        display.refresh( )

        if chosen_mode != -1:
            if chosen_mode < 3:
                from_city_id, to_city_id = display.handle_pair_of_cities_input( )

                if from_city_id == -1:
                    break
                else:
                    display.clear( )
                    display.refresh( )

                    restricted_transport = display.handle_restricted_transport_list_input( )

                    display.clear( )
                    display.refresh( )

                    if chosen_mode == 0:
                        shortest_paths, _ = dijkstra(graph,
                                                     from_city_id,
                                                     restricted_transport)
                        
                        path_str = path_to_string(shortest_paths, 
                                                  from_city_id, 
                                                  to_city_id, 
                                                  display.city_ids_store,
                                                  display.transport_ids_store)
                        
                        display.print_result("Искомый путь", path_str)

                    elif chosen_mode == 1:
                        shortest_paths, _ = dijkstra(graph,
                                                     from_city_id,
                                                     restricted_transport,
                                                     only_min_fare=True)
                        
                        path_str = path_to_string(shortest_paths, 
                                                  from_city_id, 
                                                  to_city_id, 
                                                  display.city_ids_store,
                                                  display.transport_ids_store)
                        
                        display.print_result("Искомый путь", path_str)
                    
                    elif chosen_mode == 2:
                        shortest_paths = bfs(graph,
                                             from_city_id,
                                             restricted_transport)
                        
                        path_str = path_to_string(shortest_paths, 
                                                  from_city_id, 
                                                  to_city_id, 
                                                  display.city_ids_store,
                                                  display.transport_ids_store)
                        
                        display.print_result("Искомый путь", path_str) 

            else:
                from_city_id = display.handle_single_city_input()

                display.clear()
                display.refresh()

                limit = 0

                if chosen_mode == 3:
                    limit = display.handle_cost_limit_input()
                else:
                    limit = display.handle_time_limit_input()

                display.clear()
                display.refresh()

                restricted_transport = display.handle_restricted_transport_list_input()

                display.clear()
                display.refresh()

                if chosen_mode == 3:
                    shortest_paths, reachable_cities = dijkstra(graph,
                                                                from_city_id,
                                                                restricted_transport,
                                                                limit_fare=limit)
                    
                    path_str = limit_paths_to_string(shortest_paths,
                                                     reachable_cities,
                                                     from_city_id,
                                                     city_ids_store,
                                                     transport_ids_store)
                    
                    display.print_result("Доступные маршруты:", path_str)

                elif chosen_mode == 4:
                    shortest_paths, reachable_cities = dijkstra(graph,
                                                                from_city_id,
                                                                restricted_transport,
                                                                limit_time=limit)
                    
                    path_str = limit_paths_to_string(shortest_paths,
                                                     reachable_cities,
                                                     from_city_id,
                                                     city_ids_store,
                                                     transport_ids_store)
                    
                    display.print_result("Доступные маршруты:", path_str)

        else:
            break

        c = display.stdscr.getch()

        while c != 10:
            c = display.stdscr.getch()
        
        display.clear()
        display.refresh()

        chosen_mode = display.handle_finish_app()

        if chosen_mode == 0:
            proccess_in_interaction = False

    display.endwin()
    return finish_res


                        
def main():
    res = cs.wrapper(launch)
    print(res)

if __name__ == '__main__':
    main()
                        
