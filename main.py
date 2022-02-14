import time
import threading
from os.path import exists as file_exists
from pynput.keyboard import Listener, Key
from tkinter import *
iteration = -1
top = Tk()
DIMENSION=250
canvas = Canvas(top, bg="white", height=DIMENSION, width=DIMENSION, )
line_size=Entry(top,bd=5)
side_length=10
rate = 1
def print_square(color,x,y):
    x*=side_length
    y*=side_length
    if color == '1':
        canvas.create_rectangle(x,y,x+side_length,y+side_length, fill = 'black')
    elif color == '0':
        canvas.create_rectangle(x,y,x+side_length,y+side_length,fill = "white")
    elif color == 'E':
        canvas.create_rectangle(x, y, x + side_length, y + side_length, fill="yellow")
    else:
        canvas.create_rectangle(x,y,x+side_length,y+side_length,fill = "red")
def print_maze(board):
    canvas.delete("all")
    s = board.split("\n");
    fin = []
    for row in s:
        fin.append(row.split())
    x = len(fin)
    DIMENSION = x*side_length
    top.rowconfigure(0, weight=1)
    top.columnconfigure(0, weight=1)
    canvas.config(width=DIMENSION+60, height=DIMENSION+60)
    canvas.grid(row=0, column=0, sticky='nsew')
    for i in range(0,x-1):
        y = len(fin[i])
        for f in range(0,y-1):
            print_square(fin[i][f],i,f+3)

def maze_change():
    global iteration,rate
    if iteration == -1:
        file = open("maze_iterations/maze.txt")
        board = file.read()
        print_maze(board)
    else:
        file_name = "maze_iterations/maze"+str(iteration)+".txt"
        if file_exists(file_name):
            file = open(file_name)
            board = file.read()
            print_maze(board)
        else:
            if iteration == -2:
                iteration+=1
            else:
                iteration-=1
            rate = 0
    left = canvas.create_rectangle(0, 0, 30, 30, fill="blue")
    right = canvas.create_rectangle(30, 0, 60, 30, fill="orange")
    canvas.pack()
def check(event):
    global iteration
    x = event.x
    y = event.y
    if x>=0 and x<=30 and y>=0 and y<=30:
        iteration-=1
        maze_change()
    elif x>=30 and y>=0 and x<=60 and y<=30:
        iteration +=1
        maze_change()


def main():

    top.title("reeetard")
    file = open("maze_iterations/maze.txt")


    board = file.read()
    print_maze(board)
    left = canvas.create_rectangle(0, 0, 30, 30, fill="blue")
    right = canvas.create_rectangle(30, 0, 60, 30, fill="orange")
    canvas.bind("<Button-1>",check)
    canvas.pack()
    top.mainloop()
main()