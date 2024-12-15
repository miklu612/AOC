extends Node2D

@export var output_label: Label
@export var gps_output_label: Label

var initial_block_state: Array[Vector2]
var initial_robot_state: Vector2
var initial_moves_state: String
var initial_walls_state: Array[Vector2]

var c_block: Array[Vector2]
var c_robot: Vector2
var c_moves: String
var c_walls: Array[Vector2]

var grid_w = 0
var grid_h = 0

var do_fast = false

@onready var timer: Timer = $Timer

func _process(_delta):
	if do_fast:
		timer.stop()
		update()

func toggle_fast():
	do_fast = not do_fast

func setup(blocks: Array[Vector2], robot: Vector2, moves: String, walls: Array[Vector2]) -> void:
	initial_block_state = blocks.duplicate()
	initial_robot_state = robot
	initial_moves_state = moves
	initial_walls_state = walls
	
	c_block = blocks.duplicate()
	c_robot = robot
	c_moves = moves
	c_walls = walls
	
	for wall in walls:
		grid_w = max(grid_w, wall.x)
		grid_h = max(grid_h, wall.y)
	grid_w += 1
	grid_h += 1
	render()
	
func render() -> void:
	var output = ""
	for i in range(grid_h):
		for j in range(grid_w):
			if Vector2(j, i) in c_block:
				output += "O"
			elif Vector2(j, i) == c_robot:
				output += "@"
			elif Vector2(j, i) in c_walls:
				output += "#"
			else:
				output += " "
		output += "\n"
	output_label.text = output

func move_block(block: Vector2, movement: Vector2) -> bool:
	if block + movement in c_block:
		if move_block(block + movement, movement):
			c_block[c_block.find(block)] += movement
			return true
		else:
			return false
	elif block + movement in c_walls:
		return false
	c_block[c_block.find(block)] += movement
	return true

func update() -> void:
	if len(c_moves) == 0:
		return
	else:
		var move = c_moves[0] 
		c_moves = c_moves.substr(1)
		
		var move_vector: Vector2
		
		if move == '<':
			move_vector = Vector2.LEFT
		elif move == ">":
			move_vector = Vector2.RIGHT
		elif move == "^":
			move_vector = Vector2.UP
		elif move == "v":
			move_vector = Vector2.DOWN
		else:
			print("Unknown move: " + move)
			return
			
		if c_robot + move_vector in c_walls:
			pass
		elif c_robot + move_vector in c_block:
			if move_block(c_robot + move_vector, move_vector):
				c_robot += move_vector
		else:
			c_robot += move_vector
		
		var gps_coordinates = 0
		for block in c_block:
			gps_coordinates += block.y * 100 + block.x
		gps_output_label.text = str(gps_coordinates)
		render()
		
func insta_solve() -> void:
	while len(c_moves) > 0:
		var move = c_moves[0] 
		c_moves = c_moves.substr(1)
		
		var move_vector: Vector2
		
		if move == '<':
			move_vector = Vector2.LEFT
		elif move == ">":
			move_vector = Vector2.RIGHT
		elif move == "^":
			move_vector = Vector2.UP
		elif move == "v":
			move_vector = Vector2.DOWN
		else:
			print("Unknown move: " + move)
			return
			
		if c_robot + move_vector in c_walls:
			pass
		elif c_robot + move_vector in c_block:
			if move_block(c_robot + move_vector, move_vector):
				c_robot += move_vector
		else:
			c_robot += move_vector
			
	var gps_coordinates = 0
	for block in c_block:
		gps_coordinates += block.y * 100 + block.x
	gps_output_label.text = str(gps_coordinates)
	render()

func toggle_timer() -> void:
	if timer.is_stopped():
		timer.start()
	else:
		timer.stop()
