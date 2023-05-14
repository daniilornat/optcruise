from idStore import IDStore
from Graph import *

def path_to_string(shortest_paths,
                   from_city_id,
                   to_city_id,
                   city_ids_store,
                   transport_ids_store):
    
    path = shortest_paths.get_path( from_city_id, to_city_id )

    if path[0].from_city_id == -1:
        return "Город назначения недостижим из города отправления"

    path_str = ""

    for edge in path:
        from_city_title = city_ids_store.get_name( edge.from_city_id )
        to_city_title = city_ids_store.get_name( edge.to_city_id )
        transport_title = transport_ids_store.get_name( edge.transport_id )
        time_str = str( edge.cruise_time )
        fare_str = str( edge.cruise_fare )

        path_str += from_city_title + "---->" + \
                    to_city_title + \
                    "(Время: " + \
                    time_str + \
                    " | Цена: " + \
                    fare_str +\
                    " | Транспорт: " + \
                    transport_title + \
                    ")\n"
        
    return path_str


def limit_paths_to_string(shortest_paths,
                          reachable_cities,
                          from_city_id,
                          city_ids_store,
                          transport_ids_store):
    path_str = ""
    for to_city_id in reachable_cities:
        to_city_title = city_ids_store.get_name( to_city_id )
        path_str += "Путь до города " + \
                    to_city_title + \
                    ":\n" + \
                    path_to_string(shortest_paths, from_city_id, to_city_id, city_ids_store, transport_ids_store) + \
                    "\n\n"

    return path_str               