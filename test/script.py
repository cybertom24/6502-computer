file = open("rom.bin", "wb")

for i in range(32768):
    b = i % 256
    file.write(b.to_bytes(1, byteorder='big'))

file.close()