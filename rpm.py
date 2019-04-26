def accel(x):
    return round((1 * x)/10)

def decel(x):
        return round(-2 * (x**3))

rpm = []

lastRpm = 1000

for t in range (50, 300):
        lastRpm += accel(t)
        rpm.append(lastRpm)



for gear in range (2, 6):

        for t in range (250, 370):
                lastRpm -= accel(t)
                rpm.append(lastRpm)

        for t in range (100, 300):
                lastRpm += accel(t)
                rpm.append(lastRpm)

# print("{")
# for t in time:
#     print(str(t) + ", ")

# print("}")

print("int rpms[] = {")
for t in rpm:
    print(str(t) + ", ")

print("};")
    