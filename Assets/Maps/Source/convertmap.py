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
				line += ',' + str(object["name"])
				line += ',' + str(object["x"])
				line += ',' + str(object["y"])
				if object["type"] == 'Ramp':
					points = object["polygon"]
					for i in range(len(points)):
						line += ',' + str(points[i]['x']) + ',' + str(points[i]['y'])
				else:
					line += ',' + str(object["width"])
					line += ',' + str(object["height"])
					if object["type"] == "Platform":
						customProperties = object["properties"]
						line += ',' + str(customProperties[0]["value"]) # BottomLimit
						line += ',' + str(customProperties[2]["value"]) # Velocity
						line += ',' + str(customProperties[3]["value"]) # Color
						line += ',' + str(customProperties[1]["value"]) # TopLimit
					else:
						line += ",,,,"
				outFile.write(line + '\n')
