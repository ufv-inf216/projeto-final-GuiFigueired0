import json
level = input()
fileInputName = f'Map{level}.json'
fileOutputName = f'../Map{level}_Objects.csv'

with open(fileInputName, 'r') as file:
	data = json.load(file)
	for layer in data["layers"]:
		if "objects" in layer:
			outFile = open(fileOutputName, 'w')
			for object in layer["objects"]:
				line = object["type"] + ','
				line += str(object["x"] * 2) + ','
				line += str(object["y"] * 2) + ','
				line += str(object["width"] * 2) + ','
				line += str(object["height"] * 2) + ','
				if object["type"] == 'Polygon':
					points = object["polygon"]
					for i in range(len(points)):
						line += str(points[i]['x']) + ' ' + str(points[i]['y']) + ','
				else:
					line += ",,,"
				outFile.write(line + '\n')
