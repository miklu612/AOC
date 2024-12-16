#ifndef direction_hpp
#define direction_hpp

enum class Direction {
    Up = 0,
    Right = 1,
    Down = 2,
    Left = 3,
    Unknown = 4,
};


constexpr
Direction rotate_counterclock(const Direction& direction) {
    if(direction == Direction::Unknown) {
        return Direction::Unknown;
    }
    else {
        switch(direction) {
            case Direction::Up:
                return Direction::Left;
            case Direction::Right:
                return Direction::Up;
            case Direction::Down:
                return Direction::Right;
            case Direction::Left:
                return Direction::Down;
            default:
                return Direction::Unknown;
        };
    }
}

constexpr 
Direction rotate_clockwise(const Direction& direction) {
    int i_direction = (int) direction;
    i_direction += 1;
    i_direction %= 4;
    return (Direction) i_direction;
}

constexpr
int get_turn_count(const Direction& from, const Direction& to) {
    const int from_i = (int) from;
    const int to_i = (int) to;
    if(from_i > to_i) {
        return from_i - to_i;
    }
    else {
        return to_i - from_i;
    }
}

#endif