# read the file 
with open('video.txt', 'r') as file:
    data = file.read()

# clean data, remove space {} and ,
data = data.strip('{}').replace('\n', '').replace(' ', '').split(',')

# convert data from hex string to bytes
cleaned_data = [byte.strip() for byte in data if byte.strip().startswith('0x')]

# convert data from hex to byte
byte_data = bytes(int(byte, 16) for byte in cleaned_data)

# write bytes into jpeg file
with open('output.jpg', 'wb') as image_file:
    image_file.write(byte_data)

print("JPEG generate ok and output file is output.jpg")

