class Edge:
    def __init__( self, from_city_id=-1, to_city_id=-1, transport_id=-1, cruise_time=-1, cruise_fare=-1 ):
        self.from_city_id = from_city_id
        self.to_city_id = to_city_id
        self.transport_id = transport_id
        self.cruise_time = cruise_time
        self.cruise_fare = cruise_fare

    
class AdjVer:
    def __init__( self, to_city_id, transport_id, cruise_time, cruise_fare ):
        self.to_city_id = to_city_id
        self.transport_id = transport_id
        self.cruise_time = cruise_time
        self.cruise_fare = cruise_fare

class Graph:
    def __init__( self, edges=[] ):
        self.adjList = dict( )

        for edge in edges:
            from_city_id = edge.from_city_id
            to_city_id = edge.to_city_id
            transport_id = edge.transport_id
            cruise_time = edge.cruise_time
            cruise_fare = edge.cruise_fare

            if from_city_id in self.adjList.keys():
                self.adjList[from_city_id].append( AdjVer( to_city_id, transport_id, cruise_time, cruise_fare ) )
            else:
                self.adjList[from_city_id] = []
                self.adjList[from_city_id].append( AdjVer( to_city_id, transport_id, cruise_time, cruise_fare ) )

    def add_cruise( self, edge ):
        from_city_id = edge.from_city_id
        to_city_id = edge.to_city_id
        transport_id = edge.transport_id
        cruise_time = edge.cruise_time
        cruise_fare = edge.cruise_fare

        if from_city_id in self.adjList.keys():
            self.adjList[from_city_id].append( AdjVer( to_city_id, transport_id, cruise_time, cruise_fare ) )
        else:
            self.adjList[from_city_id] = []
            self.adjList[from_city_id].append( AdjVer( to_city_id, transport_id, cruise_time, cruise_fare ) )


class Paths:
    def __init__( self ):
        self.reachable_vers_paths = dict( )

    def add( self, edge ):
        to_city_id = edge.to_city_id

        # путь к to_city_id ведет через ребро edge
        self.reachable_vers_paths[to_city_id] = edge

    def get_path( self, from_city_id, to_city_id ):
        # если вершина to_city_id недостижима то она не была добавлена в пути от from_city_id
        if to_city_id not in self.reachable_vers_paths.keys():
            return [Edge( )]

        path_to_to_city = []
        while to_city_id != from_city_id:
            path_to_to_city.append( self.reachable_vers_paths[to_city_id] )
            to_city_id = self.reachable_vers_paths[to_city_id].from_city_id
        
        path_to_to_city.reverse( )

        return path_to_to_city