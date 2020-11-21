import graphO as o
from random import randint
import networkx as nx
import matplotlib.pyplot as plt
import time
from plots import Plots as pt

#points = [(0,0),(5,1),(5,3),(2,4),(2,2)]
#points = [(1,1),(1,-1),(5,1),(0,0),(2,2)]
#points = [(1,1),(1,-1),(-1,1),(-1,-1)]
n = 5 #n(n-1)/2
points = []
for i in range(n):
    points.append((randint(-1000,1000) , randint(-1000,1000)))
pt.plot_points(points = points,show_segments = True,save_image = False)
pt.plot_planes(points)
segments = o.to_segments(points)
adj_list = o.to_disjointness_graph_A(points)
pt.plot_graph(adj_list,segments)
# # #print(list(enumerate(segments)))
# print("Fuerza bruta")
# initial = time.time()
# adj_list = o.to_disjointness_graph_BF(points)
# #print(adj_list)
# print("Tiempo en segundos",time.time() - initial)
# print("----------------------------")
# print("Algoritmo A")
# initial = time.time()
# adj_list = o.to_disjointness_graph_A(points)
# #print(adj_list)
# print("Tiempo en segundos",time.time() - initial)

# #pt.plot_planes(points)
# bf = []
# A = []
# pruebas = 50
# for tam in range(1,pruebas + 1):
#     n = tam
#     points = []
#     for i in range(n):
#         points.append((randint(-1000,1000) , randint(-1000,1000)))
#     #BF
#     initial = time.time()
#     adj_list = o.to_disjointness_graph_BF(points)
#     bf.append(time.time() - initial)
#     #A
#     initial = time.time()
#     adj_list = o.to_disjointness_graph_A(points)
#     A.append(time.time() - initial)
# plt.plot(range(1, pruebas + 1),bf,label='Fuerza bruta')
# plt.plot(range(1,pruebas + 1),A,label='Algoritmo A')
# plt.title("Comparación del tiempo de ejecución entre fuerza bruta y el algoritmo A")
# plt.xticks(range(1, pruebas + 1))
# plt.xlabel('Numero de puntos |P|')
# plt.ylabel('Tiempo(s)')
# plt.legend()
# plt.show()


