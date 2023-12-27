import math

print("const uint8_t sin[180]={")
for i in range(0,180):
    ang = i*3.1415/180
    s=math.cos(ang)*127
    s=255-int(s)-128
    print(str(s),end="")
    print(",",end="")
    if (i+1)%16==15:
        print("")
print("};")
