class IDStore:
    def __init__(self):
        self.titles_to_ids = dict()
        self.ids_to_titles = dict()

        self.current_last_id = -1

    def size( self ):
        return len( self.titles_to_ids )
    
    def add_id( self, title ):
        if not self.contains_title( title ):
            self.current_last_id += 1
            self.titles_to_ids[title] = self.current_last_id
            self.ids_to_titles[self.current_last_id] = title

    def get_id( self, title ) -> int:
        return self.titles_to_ids[title]

    def get_name( self, id ) -> str:
        return self.ids_to_titles[id]

    def contains_title( self, title: str ):
        return title in self.titles_to_ids.keys( )

    def contains_id( self, id: int ):
        return id in self.ids_to_titles.keys()
