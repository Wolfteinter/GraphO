import tkinter as tk
from tkinter import ttk
from tkinter import Checkbutton, IntVar
from tkinter import filedialog
from tkinter import messagebox
from tkinter.ttk import Style
from tkinter.simpledialog import askstring
from tkinter.simpledialog import askinteger
from random import randint
import graphO as o
from random import randint
import networkx as nx
import matplotlib.pyplot as plt
import time
from plots import Plots as pt
import os

background_color = "#95b589"
opt_bar_color = "#6e7f68"
h1_color = "#90908c"
conts_btn_color = "#616d5c"
prob_btn_color = "#4682a7"
AC_color = "#61d649"
WA_color = "#ea2626"
TLI_color = "#eadb26"
points_window_width = 250
results_window_width = 800
window_height = 500
class VerticalScrolledFrame(tk.Frame):
    def __init__(self, parent,color, *args ,**kw):
        tk.Frame.__init__(self, parent, *args, **kw)
        vscrollbar = tk.Scrollbar(self, orient=tk.VERTICAL,width=20)
        vscrollbar.pack(fill=tk.Y, side=tk.RIGHT, expand=tk.TRUE)
        canvas = tk.Canvas(self, bd=0, highlightthickness=0,
                        yscrollcommand=vscrollbar.set,height = 500,bg = color)
        canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=tk.TRUE)
        vscrollbar.config(command=canvas.yview)
        canvas.xview_moveto(0)
        canvas.yview_moveto(0)
        self.interior = interior = tk.Frame(canvas)
        interior_id = canvas.create_window(0, 0, window=interior,
                                           anchor=tk.NW)
        def _configure_interior(event):
            size = (interior.winfo_reqwidth(), interior.winfo_reqheight())
            canvas.config(scrollregion="0 0 %s %s" % size)
            if interior.winfo_reqwidth() != canvas.winfo_width():
                canvas.config(width=interior.winfo_reqwidth())
        interior.bind('<Configure>', _configure_interior)
        def _configure_canvas(event):
            if interior.winfo_reqwidth() != canvas.winfo_width():
                canvas.itemconfigure(interior_id, width=canvas.winfo_width())
        canvas.bind('<Configure>', _configure_canvas)
        interior.bind_all('<MouseWheel>', lambda event:     vscrollbar.yview("scroll",event.delta,"units"))

class GUI:
    def __init__(self,master):
        self.master = master
        self.points = []
        self.initial_settings()
        self.main()
        self.master.mainloop()
    def initial_settings(self):
        self.master.title("Alpha 6")
        self.master.resizable(False,False)
        self.master.geometry("1000x500")
        self.master.config(bg=background_color)
    def main(self):
        self.points_bar = tk.Frame(self.master)
        self.points_bar.config(width = points_window_width,height=40,bg=opt_bar_color)
        self.points_bar.pack(fill = "y",side=tk.LEFT)
        self.points_bar.pack_propagate(False)
        self.points_scframe = VerticalScrolledFrame(self.points_bar,color = opt_bar_color)
        self.points_scframe.pack()
        self.points_frame()

        results_bar = tk.Frame(self.master)
        results_bar.config(width = results_window_width,height=40,bg=opt_bar_color)
        results_bar.pack(fill = "y",side=tk.RIGHT)
        results_bar.pack_propagate(False)
    def random_points(self):
        prompt = askinteger("Input", "How many points?")
        for i in range(prompt):
            self.points.append((randint(0,1000) , randint(0,1000)))
        self.points_frame()
    def add_point(self):
        prompt = askstring("Input", "Input a point x,y format")
        value = prompt.split(",")
        self.points.append((value[0],value[1]))
        self.points_frame()
    def show_results(self):
        result_list = os.listdir("results/")
        path_new = "results/" + str(len(result_list))
        os.mkdir(path_new)
        pt.plot_points(self.points,show_segments = True,save_image = True,path = path_new)
        segments = o.to_segments(self.points)
        #print(list(enumerate(segments)))
        initial = time.time()
        adj_list = o.to_disjointness_graph(self.points)
        print(len(adj_list))
        print(time.time() - initial)
        pt.plot_graph(adj_list,segments,save_image = True,path = path_new)

    def points_frame(self):
        if(len(self.points) == 0):
            info_label = tk.Label(self.points_scframe.interior,text = "No points",font=("Arial",12),width = points_window_width)
            info_label.pack()
        else:
            for widget in self.points_scframe.interior.winfo_children():
                widget.destroy()
            for point in self.points:
                index = tk.Frame(self.points_scframe.interior,width = points_window_width)
                index.pack(side = tk.TOP)
                left_l = tk.Label(index,text = "X =")
                left_l.pack(side=tk.LEFT)
                left = ttk.Entry(index,width = 9)
                left.pack(side=tk.LEFT)
                left.insert(0,str(point[0]))
                right_l = tk.Label(index,text = "Y =")
                right_l.pack(side=tk.LEFT)
                right = ttk.Entry(index,width = 9)
                right.pack(side=tk.RIGHT)
                right.insert(0,str(point[1]))
        add_button = tk.Button(self.points_scframe.interior,text = "Random Points",font=("Arial",12),width = points_window_width,command = self.random_points)
        add_button.pack()
        add_button = tk.Button(self.points_scframe.interior,text = "Add point",font=("Arial",12),width = points_window_width,command = self.add_point)
        add_button.pack()
        continue_button = tk.Button(self.points_scframe.interior,text = "Continue",font=("Arial",12),width = points_window_width,command = self.show_results)
        continue_button.pack()




root = tk.Tk()
my_gui = GUI(root)
