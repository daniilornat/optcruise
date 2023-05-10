#include <queue>
#include <unordered_map>
#include <ncurses.h>

#include "graph.hpp"
#include "interface.hpp"

#ifdef GRAPH_H

#define INF 1e6

std::vector<int> dijkstra( Graph &graph,
                           Path &shortest_path,                           // для возврата кратчайшего пути
                           const id start_ver,                            // город отправления
                           const std::set<id> restricted_transport,       // нежелательный транспорт
                           const bool only_min_fare = false,              // искать минимальные по стоимости
                           const cruiseTime time_limit = 0,               // ограничение по времени маршрута
                           const cruiseFare fare_limit = 0) {             // ограничение по стоимости маршрута

    int n = graph.num_vers;
    
    // хранят расстояния до от start_ver до вершин графа во времени и стоимости
    std::vector<int> distances_time( n, INF );
    std::vector<int> distances_fare( n, INF );

    // расстояния до start_ver считаем 0
    distances_time[start_ver] = 0;
    distances_fare[start_ver] = 0;

    typedef std::tuple<cruiseTime, cruiseFare, id> triple;
    std::priority_queue <triple, std::vector<triple>, std::greater<triple>> queue;
    queue.push( std::make_tuple( 0, 0, start_ver ) );

    while ( !queue.empty( ) ) {
        // текущие кратчайшие расстояния до ver
        auto [ cur_dist_time, cur_dist_fare, ver ] = queue.top( );
        queue.pop( );

        if ( cur_dist_time > distances_time[ver] || 
            ( cur_dist_time == distances_time[ver] && 
                cur_dist_fare > distances_fare[ver] ) )
            continue;

        for ( auto adj_ver : graph[ver] ) {
            // пропускаем рейсы с неподходящим транспортом
            id transport = adj_ver.get_transport_id( );
            if ( restricted_transport.count( transport ) )
                continue;

            id to_city = adj_ver.get_to_city_id( );           
            cruiseTime time = adj_ver.get_cruise_time( );
            cruiseFare fare = adj_ver.get_cruise_fare( );
            bool relax_ver;

            if ( only_min_fare ) {
                // обнавляем вершины только в взависимости от стоимости
                relax_ver = distances_fare[to_city] > distances_fare[ver] + fare;
            } else {
                // обнавляем вершины в зависимости от времени и стоимости
                relax_ver = distances_time[to_city] > distances_time[ver] + time || 
                                ( distances_time[to_city] == distances_time[ver] + time && 
                                    distances_fare[to_city] > distances_fare[ver] + fare );
            }

            if ( relax_ver ) {   
                            
                // обновляем расстояния
                distances_time[to_city] = distances_time[ver] + time;
                distances_fare[to_city] = distances_fare[ver] + fare;
                            
                // кратчайший путь в to_city проходит через ver
                Edge arc_for_shrt_pth( ver, to_city, transport, time, fare );
                shortest_path + arc_for_shrt_pth;
                            
                queue.push( {time, fare, to_city} );
            }
        }
    }

    // множества городов, подходящих по ограничениям
    std::vector<int> result;

    if ( time_limit ) {
        for ( int i = 0; i < n; i++ ) {
            const auto total_time = distances_time[i];

            if ( i == start_ver )
                continue;

            if ( total_time <= time_limit )
                result.push_back( i );
        }
        return result;
    }
                            
    if ( fare_limit ) {
        for ( int i = 0; i < n; i++ ) {
            const auto total_fare = distances_fare[i];

            if ( i == start_ver )
                continue;

            if ( total_fare <= fare_limit )
                result.push_back( i ); 
        }
        return result;
    }

    if (only_min_fare)
        return distances_fare;

    return distances_time;
}


// минимальные по числу посещенных городов
std::vector<int> bfs( Graph &graph,
                      Path &shortest_path,
                      const id start_ver,
                      const std::set<id> restricted_transport ) {

    int n = graph.num_vers;

    // i-й элемент хранит расстояние от start_ver до i-й вершины (в количестве пройденных городов)
    std::vector<int> distances( n, -1 );
    distances[start_ver] = 0;

    // очередь непройденных вершин
    std::queue<id> q;
    q.push( start_ver );

    while ( !q.empty( ) ) {

        // переходим в вершину ver
        int ver = q.front( );
        q.pop( );

        for ( auto adj_ver : graph[ver] ) {

            // пропускаем рейсы с неподходящим транспортом
            id transport = adj_ver.get_transport_id( );
            if ( restricted_transport.count( transport ) )
                continue;

            id to_city = adj_ver.get_to_city_id( );
            cruiseTime time = adj_ver.get_cruise_time( );
            cruiseFare fare = adj_ver.get_cruise_fare( );

            if ( distances[to_city] == -1 ) {
                q.push( to_city );
                distances[to_city] = distances[ver] + 1;

                // кратчайший путь в to_city проходит через ver
                Edge arc_for_shrt_pth( ver, to_city, transport, time, fare );
                shortest_path + arc_for_shrt_pth;
            }
        }
    }
    return distances;
}


#endif