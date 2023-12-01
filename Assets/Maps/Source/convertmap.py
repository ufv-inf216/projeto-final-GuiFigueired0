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
				line = object["type"]
				line += ',' + str(object["x"])
				line += ',' + str(object["y"])
				if object["type"] == 'Ramp':
					points = object["polygon"]
					for i in range(len(points)):
						line += ',' + str(points[i]['x']) + ',' + str(points[i]['y'])
				else:
					line += ',' + str(object["width"])
					line += ',' + str(object["height"])
					line += ",,,,"
				outFile.write(line + '\n')
