from random import randint
import networkx as nx
import matplotlib.pyplot as plt
from random import randint
from matplotlib import collections  as mc
import matplotlib

class Plots():
    def plot_points(points,show_segments = True,save_image = False,path = ""):
        if(show_segments):
            n = len(points)
            lines = []
            for i in range(n):
                for j in range(i + 1,n):
                    lines.append([points[i] , points[j]])

            lc = mc.LineCollection(lines, linewidths=2)
            fig, ax = plt.subplots()
            ax.add_collection(lc)
            ax.autoscale()
            ax.margins(0.1)
        xs,ys = zip(*points)
        for i,p in enumerate(points):
            plt.text(p[0] + 10,p[1] - 10,str(i + 1))
        plt.plot(xs,ys,'ro')
        if(save_image):
            plt.savefig(path +"/"+"points.png")
        else:
            plt.show()
        plt.clf()
    def plot_graph(adj_list,segments,save_image = False,path = ""):
        edges = []
        G=nx.Graph()
        for i in adj_list.keys():
            nodes = adj_list[i]
            for node in nodes:
                left = segments[i]
                right = segments[node]
                edges.append((str(left[0] + 1)+"-"+str(left[1] + 1) , str(right[0] + 1) +"-"+ str(right[1] + 1)))
        G.add_edges_from(edges)
        nx.draw(G , with_labels = True , font_size = 8)
        if(save_image):
            plt.savefig(path+"/"+"graph.png") # save as png
        else:
            plt.show()
        plt.clf()
    def plot_planes(points):
        xsum = 0
        ysum = 0
        xmax = float('-inf')
        xmin = float('inf')
        ymax = float('-inf')
        ymin = float('inf')
        for p in points:
            xsum += p[0]
            ysum += p[1] 
            xmax = max(xmax , p[0])
            xmin = min(xmin , p[0])
            ymax = max(ymax , p[1])
            ymin = min(ymin , p[1])
        xmin -= 100
        ymin -= 100
        xmax += 100
        ymax += 100
        xsum /= len(points)
        ysum /= len(points)
        print(xsum,ysum)
        fig = plt.figure()
        ax = fig.add_subplot(111)
        plt.xlim([xmin, xmax])
        plt.ylim([ymin, ymax])
        rect1 = matplotlib.patches.Rectangle((xmin,ymin), xsum - xmin, ysum - ymin, color='yellow',alpha=.3)
        rect2 = matplotlib.patches.Rectangle((xsum,ysum), xmax - xsum, ymax - ysum, color='red', alpha=.3)
        rect3 = matplotlib.patches.Rectangle((xmin,ysum), xsum - xmin,ymax - ysum, color='blue', alpha=.3)
        rect4 = matplotlib.patches.Rectangle((xsum,ymin), xmax - xsum, ysum - ymin, color='green', alpha=.3)
        #rect2 = matplotlib.patches.Rectangle((200,-100), 400, 200, color='red')
        #rect3 = matplotlib.patches.Rectangle((-200,100), 400, 200, color='green')
        #rect4 = matplotlib.patches.Rectangle((200,100), 400, 200, color='blue')
        ax.add_patch(rect1)
        ax.add_patch(rect2)
        ax.add_patch(rect3)
        ax.add_patch(rect4)
        plt.plot([x[0] for x in points], [x[1] for x in points],'ro')
        plt.show() 
