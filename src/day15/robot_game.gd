extends Node

@export var output_label: Label

func show_input_loader() -> void:
	var file_dialog: FileDialog = FileDialog.new()
	file_dialog.file_mode = FileDialog.FILE_MODE_OPEN_FILE
	file_dialog.file_selected.connect(load_input)
	file_dialog.show()
	add_child(file_dialog)
	file_dialog.size.y = 300
	
func load_input(path: String) -> void:
	
	var file := FileAccess.open(path, FileAccess.READ)
	var text := file.get_as_text()
	var lines := text.split("\n")
	
	var blocks: Array[Vector2]
	var robot: Vector2
	var walls: Array[Vector2]
	
	var new_index = -1
	for i in range(len(lines)):
		var line = lines[i]
		if line.is_empty():
			new_index = i
			break
		else:
			for j in range(len(line)):
				if line[j] == '#':
					walls.push_back(Vector2(j, i))
				elif line[j] == "O":
					blocks.push_back(Vector2(j, i))
				elif line[j] == '@':
					robot = Vector2(j, i)
	
	new_index += 1
	
	var moves = ""
	for i in range(new_index, len(lines)):
		var line = lines[i]
		if line.is_empty():
			break
		else:
			moves += line
			
	$Game.setup(blocks, robot, moves, walls)


func _on_h_slider_value_changed(value):
	var label_settings = LabelSettings.new()
	label_settings.font_size = value
	output_label.label_settings = label_settings
