#include <string>

#include "graph.hpp"

// для вывода пути от from_city в to_city в файл и на экран
std::string path_to_string( Path &path, 
                            const id &from_city, 
                            const id &to_city, 
                            const IDStore &city_id_store,
                            const IDStore &transport_id_store ) {

    // ребра искомого пути (от конца до начала)
    std::vector<Edge> path_arcs;
    path.get_path( from_city, to_city, path_arcs );

    // искомый путь в виде строки
    std::string path_str = "";

    std::string temp_from_city_str;
    std::string temp_to_city_str;
    std::string temp_transport_str;
    std::string temp_time_str;
    std::string temp_fare_str;

    // ребро в виде строки
    std::string temp_edge_str;

    for ( std::vector<Edge>::reverse_iterator it = path_arcs.rbegin( ); it != path_arcs.rend( ); ++it ) {

        temp_from_city_str = city_id_store.get( (*it).from_city );
        temp_to_city_str = city_id_store.get( (*it).to_city );
        temp_transport_str = transport_id_store.get( (*it).transport_type );
        temp_time_str = std::to_string( (*it).time );
        temp_fare_str = std::to_string( (*it).fare );

        temp_edge_str = temp_from_city_str + " -----> " 
                                           + temp_to_city_str
                                           + " (Время: "
                                           + temp_time_str
                                           + " | Цена: "
                                           + temp_fare_str
                                           + " | Транспорт: "
                                           + temp_transport_str
                                           + ")";

        path_str += temp_edge_str + "\n";
    }

    return path_str;
}

std::string limit_paths_to_string( Path &paths,
                                   const std::vector<id> available_cities,
                                   const id &from_city,
                                   const IDStore &city_id_store,
                                   const IDStore &transport_id_store ) {

    std::string path_str = "";

    for (const auto to_city_id : available_cities) {
        std::string to_city_name = city_id_store.get( to_city_id );

        path_str += "Путь до города " 
                    + to_city_name
                    + ":\n"
                    + path_to_string( paths, 
                                      from_city,
                                      to_city_id,
                                      city_id_store,
                                      transport_id_store )
                    + "\n\n";         
    }
    return path_str;
}