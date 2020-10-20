import graphO as o
#points = [(0,0),(5,1),(5,3),(2,4),(2,2)]
points = [(2,4),(5,3),(5,1),(0,0),(2,2)]
segments = o.to_segments(points)
print(list(enumerate(segments)))
print(o.to_disjointness_graph(points))
