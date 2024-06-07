from tkinter import *
from PIL import Image, ImageTk

def play(n,m):
    # login_image = Image.open('test.jpg')
    # login_image = ImageTk.PhotoImage(login_image)

    # images = [login_image for _ in range(10)]
    toggles = [" ","X","Y","W","P","G","S","L","B","H"]

    def right(ind):
        index_grid[ind] = (index_grid[ind] + 1)%len(toggles)
        btn_grid[ind].config(text=f"{toggles[index_grid[ind]]}")
        # btn_grid[ind].config(image=images[index_grid[ind]])

    def on_button_click():
        f = open("level.txt", "w")
        for row in range(n):
            s = ""
            for col in range(m):
                if toggles[index_grid[m*row+col]]!=" ":
                    s=s+(toggles[index_grid[m*row+col]])
                else:
                    s=s+"."
            print(s)
            f.write(s)
            f.write("\n")
        f.close()
    
    win = Tk()

    main_frame = Frame(win)
    main_frame.pack()

    btn_grid = list()
    index_grid = list()
    i = 0

    for row in range(n):
        for col in range(m):
            btn_grid.append(Button(main_frame, text=toggles[0], height=1, width=1, font="Verdana", relief='groove'))
            btn_grid[i].grid(row=row, column=col, sticky="news", ipadx=20, ipady=15)
            btn_grid[i].bind("<Button-1>", lambda e, c=i: right(c))
            i = i + 1
            index_grid.append(0)

    submit_button = Button(main_frame,text="Submit")
    submit_button.grid(row=n+1,column=0)
    submit_button.bind("<Button-1>", lambda e, c=i: on_button_click())

    win.mainloop()

print("Enter row and column on separate lines (2n+1 and 2m+1):")
play(int(input()),int(input()))