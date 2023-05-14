from Graph import *
from math import inf
from queue import Queue

import curses as cs
import heapq as hq


def dijkstra(
    graph,
    start_city,
    restricted_transport,
    only_min_fare=False,
    limit_time=0,
    limit_fare=0):

    num_vers = len( graph.adjList )

    shortest_paths = Paths()

    distances_in_time = [inf] * num_vers
    distances_in_fare = [inf] * num_vers

    distances_in_time[start_city] = 0
    distances_in_fare[start_city] = 0

    queue = []
    hq.heapify( queue )

    hq.heappush( queue, ( distances_in_time[start_city], distances_in_fare[start_city], start_city ) )

    while len(queue) != 0:
        curr_dist_time, curr_dist_fare, curr_city_id = hq.heappop( queue )

        if only_min_fare and ( curr_dist_fare > distances_in_fare[curr_city_id] ):
            continue

        if curr_dist_time > distances_in_fare[curr_city_id] or \
                (curr_dist_time == distances_in_fare[curr_city_id] and \
                    curr_dist_fare > distances_in_fare[curr_city_id]):
            continue

        for adj_ver in graph.adjList[curr_city_id]:
            transport_id = adj_ver.transport_id

            if transport_id in restricted_transport:
                continue

            adj_city_id = adj_ver.to_city_id
            cruise_time = adj_ver.cruise_time
            cruise_fare = adj_ver.cruise_fare

            if only_min_fare:
                need_relax_ver = distances_in_fare[adj_city_id] > distances_in_fare[curr_city_id] + cruise_fare
            else:
                need_relax_ver = distances_in_time[adj_city_id] > distances_in_time[curr_city_id] + cruise_time or \
                    (distances_in_time[adj_city_id] == distances_in_time[curr_city_id] + cruise_time and \
                        distances_in_fare[adj_city_id] > distances_in_fare[curr_city_id] + cruise_fare)

            if need_relax_ver:
                distances_in_time[adj_city_id] = distances_in_time[curr_city_id] + cruise_time
                distances_in_fare[adj_city_id] = distances_in_fare[curr_city_id] + cruise_fare

                shortest_paths.add( Edge( curr_city_id, adj_city_id, transport_id, cruise_time, cruise_fare ) )

                hq.heappush( queue, ( cruise_time, cruise_fare, adj_city_id ) )


    reachable_by_limits = []

    if limit_time:
        for id in range( len( distances_in_time ) ):
            if id == start_city:
                continue

            total_time = distances_in_time[id]

            if total_time <= limit_time:
                reachable_by_limits.append( id )

        return shortest_paths, reachable_by_limits
    
    if limit_fare:
        for id in range( len( distances_in_fare ) ):
            if id == start_city:
                continue

            total_fare = distances_in_fare[id]

            if total_fare <= limit_fare:
                reachable_by_limits.append( id )

        return shortest_paths, reachable_by_limits

    return shortest_paths, reachable_by_limits

def bfs( graph,
         start_city,
         restricted_transport ):

    num_vers = len( graph.adjList )

    shortest_paths = Paths()

    distances = [-1] * num_vers
    distances[start_city] = 0

    q = Queue( )
    q.put( start_city )

    while not q.empty( ):
        curr_city_id = q.get( )

        for adj_ver in graph.adjList[curr_city_id]:
            transport_id = adj_ver.transport_id

            if transport_id in restricted_transport:
                continue

            adj_city_id = adj_ver.to_city_id
            cruise_time = adj_ver.cruise_time
            cruise_fare = adj_ver.cruise_fare

            if distances[adj_city_id] == -1:
                q.put( adj_city_id )
                distances[adj_city_id] = distances[curr_city_id] + 1

                shortest_paths.add( Edge( curr_city_id, adj_city_id, transport_id, cruise_time, cruise_fare ) )

    return shortest_paths