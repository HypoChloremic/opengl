import re

def process_osm(input_file, output_file): 
    print("Opening ", input_file)
    with open(input_file, "rb") as file:
        filedata = file.read()

    filtered = re.findall(rb'(?<=\slat="|\slon=")[0-9\.-]+(?=")', filedata)

    print("Writing ", input_file)
    with open(output_file, "wb") as file:
        file.write(b" ".join(filtered))

if __name__ == "__main__":
    input_file = "./data/botkyrka.osm"
    output_file = "./data/raw.bin"
    try:
        process_osm(input_file, output_file)
    except Exception as e:
        print("Error while processing data")
        raise e
