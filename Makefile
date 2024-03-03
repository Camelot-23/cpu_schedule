all:main

main:src/main.c src/task.c src/file.c src/fcfs.c src/nsjf.c src/rr.c src/psjf.c
	gcc -o cpu_schedule src/main.c src/task.c src/file.c src/fcfs.c src/nsjf.c src/rr.c src/psjf.c -Iinclude

clean:
	rm cpu_schedule
