import threading
import time
import numpy

request = 0
respond = 0
soma = 0
lock = threading.Lock()

def critical_section():
    global soma
    time.sleep(numpy.random.rand()%2)
    soma += 1
    

def servidor():
    global request
    global respond
    
    while(1):
        while(request == 0):
            continue
        respond = request		
        while(respond != 0):
            continue
        request = 0


def cliente(i):
    global request
    global respond
    global soma
    while(1):
        while(respond != i):
            request=i
        with lock:
            critical_section()
            print(soma,"-- Thread",i)
            respond = 0

def cliente2(i):
    global soma
    while(1):
        
        soma += 1
        print(soma,"-- Thread",i)


x = threading.Thread(target=servidor)
x.start()

for i in range(2):
    x = threading.Thread(target=cliente, name=i, args=(i+1,))
    x.start()
    