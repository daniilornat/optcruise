#include "graph.h"
#include <queue>

#ifdef GRAPH_H

#define INF 1e6

// для поиска минимальных по стоимости среди минимальных по времени
void dijkstra( Graph &graph,
               Path &shortest_path,                            // для возврата кратчайшего пути
               const city_id start_ver,                        // город отправления
               const std::set<transport_id> allowed_transport, // используемый пользователем транспорт
               const cruise_time time_limit = 0,               // ограничение по времени маршрута
               const cruise_fare fare_limit = 0) {             // ограничение по стоимости маршрута

    int n = graph.num_vers;
    
    // хранят расстояния до от start_ver до вершин графа во времени и стоимости
    std::vector<int> distances_time( n, INF );
    std::vector<int> distances_fare( n, INF );

    // расстояния до start_ver считаем 0
    distances_time[start_ver] = 0;
    distances_fare[start_ver] = 0;

    typedef std::tuple<cruise_time, cruise_fare, city_id> triple;
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
            transport_id transport = adj_ver.get_transport_id( );
            if ( !allowed_transport.count( transport ) )
                continue;

            city_id to_city = adj_ver.get_to_city_id( );           
            cruise_time time = adj_ver.get_cruise_time( );
            cruise_fare fare = adj_ver.get_cruise_fare( );

            if ( distances_time[to_city] > distances_time[ver] + time || 
                ( distances_time[to_city] == distances_time[ver] + time && 
                    distances_fare[to_city] > distances_fare[ver] + fare ) ) {
                         
                        // пропускаем неподходящие по ограничениям пути
                        if ( time_limit != 0 )
                            if ( time_limit < distances_time[ver] + time )
                                continue;
                        
                        if ( fare_limit != 0)
                            if ( fare_limit < distances_fare[ver] + fare )
                                continue;
                        
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
}


// минимальные по числу посещенных городов
void dfs( Graph &graph,
          Path &shortest_path,
          const city_id start_ver,
          const std::set<transport_id> allowed_transport) {

    int n = graph.num_vers;

    // i-й элемент хранит расстояние от start_ver до i-й вершины (в количестве пройденных городов)
    std::vector<int> distances( n, -1 );
    distances[start_ver] = 0;

    // очередь непройденных вершин
    std::queue<int> q;
    q.push( start_ver );

    while ( !q.empty( ) ) {
        // переходим в вершину ver
        int ver = q.front( );
        q.pop( );

        for ( auto adj_ver : graph[ver] ) {

            // пропускаем рейсы с неподходящим транспортом
            transport_id transport = adj_ver.get_transport_id( );
            if ( !allowed_transport.count( transport ) )
                continue;

            city_id to_city = adj_ver.get_to_city_id( );
            cruise_time time = adj_ver.get_cruise_time( );
            cruise_fare fare = adj_ver.get_cruise_fare( );

            if ( distances[to_city] == -1 ) {
                q.push( to_city );
                distances[to_city] = distances[ver] + 1;
                
                // кратчайший путь в to_city проходит через ver
                Edge arc_for_shrt_pth( ver, to_city, transport, time, fare );
                shortest_path + arc_for_shrt_pth;
            }
        }
    }
}


#endif