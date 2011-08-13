#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

#define NORTH 'N'
#define SOUTH 'S'
#define EAST  'E'
#define WEST  'W'

#define FOOD  'f'
#define ANT   'a'
#define WATER 'w'
#define EMPTY '.'

struct Ant {
  int x;
  int y;
  int player;
  int move;
};

struct Food {
  int x;
  int y;
};

struct Cell {
  char type;
  char player;
};

struct Game {
  int loadtime;
  int turntime;
  int rows;
  int cols;
  int turns;
  int vr;
  int ar;
  int sr;

  Cell *map;

  Ant  ants[1000];
  int  ant_tail;

  Food food[1000];
  int  food_tail;
};

struct Input {
  char line[4096];
  FILE *inp;
};

struct Output {
  FILE *outp;
};

Game* new_game() {
  return (Game *)malloc(sizeof(Game));
}

Input* new_input(FILE *inp) {
  Input *input =  (Input *) malloc(sizeof(Input));
  input->inp = inp;
  return input;
}

Output* new_output(FILE *outp) {
  Output *output =  (Output *) malloc(sizeof(Output));
  output->outp = outp;
  return output;
}

void initialize_game(Game *game) {
  game->map = (Cell *)malloc(sizeof(Cell) * game->rows *game->cols);
  game->ant_tail = 0;
  game->food_tail = 0;
}

void initialize_turn(Game *game) {
  memset(game->map, sizeof(Cell)*game->rows*game->cols, 0);
  game->ant_tail = 0;
  game->food_tail = 0;
}

char const* read_line(Input *input) {
  return fgets(input->line, sizeof(input->line), input->inp);
}

Cell* get_cell(Game *game, int x, int y) {
  return game->map + (y*game->rows) + x;
}

void insert_food(Game *game, int x, int y) {
  get_cell(game, x, y)->type = FOOD;
  Food *food = game->food + game->food_tail++;
  food->x = x;
  food->y = y;
  
}

void remove_food(Game *game, int x, int y) {
  get_cell(game, x, y)->type = EMPTY;
}

void remove_ant(Game *game, int x, int y, char player) {
  get_cell(game, x, y)->type = EMPTY;
}

void insert_ant(Game *game, int x, int y, char player) {
  Cell *cell = get_cell(game, x, y);
  Ant *ant = game->ants + game->ant_tail++;
  cell->type = ANT;
  cell->player = player;
  ant->x = x;
  ant->y = y;
  ant->player = player;
}

void insert_water(Game *game, int x, int y) {
  get_cell(game, x, y)->type = WATER;
}

char const* starts_with(Input *input, char const*s) {
  char const*p = input->line;
  while(*s && *p && *p == *s) ++p, ++s;
  return (*s == 0) ? p : 0;
}

char const* parse_int(char const*s, int *p) {
  int val = 0;
  while( isspace(*s) ) ++s;
  while( isdigit(*s) ) {
    val *= 10;
    val += *s - '0';
    ++s;
  }
  *p = val;
  return s;
}

char const*parse_int_pair(char const*s, int *px, int *py) {
  s = parse_int(s, px);
  s = parse_int(s, py);
  return s;
}

char const*parse_int_triple(char const*s, int *px, int *py, int *pz) {
  s = parse_int(s, px);
  s = parse_int(s, py);
  s = parse_int(s, pz);
  return s;
}

char const*parse_insert_food(char const*s, Game *game) {
  int x, y;
  s = parse_int_pair(s, &x, &y);
  insert_food(game, x, y);
  return s;
}

void parse_remove_food(char const*s, Game *game) {
  int x, y;
  s = parse_int_pair(s, &x, &y);
  remove_food(game, x, y);
}

char const*parse_insert_water(char const*s, Game *game) {
  int x, y;
  s = parse_int_pair(s, &x, &y);
  insert_water(game, x, y);
  return s;
}

char const*parse_insert_ant(char const*s, Game *game) {
  int x, y, player;
  s = parse_int_triple(s, &x, &y, &player);
  insert_ant(game, x, y, player);
  return s;
}

char const*parse_remove_ant(char const*s, Game *game) {
  int x, y, player;
  s = parse_int_triple(s, &x, &y, &player);
  remove_ant(game, x, y, player);
  return s;
}

int parse_game_turn(Input *input, Game *game) {
  char const*s;
  while(read_line(input)) {
    if ( starts_with(input, "turn") ) {
      break;
    }
    else if ( starts_with(input, "end") ) {
      return 0;
    }
  }

  while(read_line(input) ) {
    if ( (s = starts_with(input, "f ")) ) {
      parse_insert_food(s, game);
    }
    if ( (s = starts_with(input, "f ")) ) {
      parse_insert_water(s, game);
    }
    if ( (s = starts_with(input, "a ")) ) {
      parse_insert_ant(s, game);
    }
    if ( (s = starts_with(input, "r ")) ) {
      parse_remove_food(s, game);
    }
    if ( (s = starts_with(input, "d ")) ) {
      parse_remove_ant(s, game);
    }
    if ( (s = starts_with(input, "go")) ) {
      break;
    }
  }
  return 1;
}

void parse_game_description(Input *input, Game *game) {
  char const*s;
  while(read_line(input) && ! starts_with(input, "ready") ) {
    if ( (s = starts_with(input, "loadtime ")) ) {
      parse_int(s, &game->loadtime);
    }
    else if ( (s = starts_with(input, "turntime")) ) {
      parse_int(s, &game->turntime);
    }
    else if ( (s = starts_with(input, "rows")) ) {
      parse_int(s, &game->rows);
    }
    else if ( (s = starts_with(input, "cols")) ) {
      parse_int(s, &game->cols);
    }
    else if ( (s = starts_with(input, "turns")) ) {
      parse_int(s, &game->turns);
    }
    else if ( (s = starts_with(input, "viewradius2")) ) {
      parse_int(s, &game->vr);
    }
    else if ( (s = starts_with(input, "attackradius2")) ) {
      parse_int(s, &game->ar);
    }
    else if ( (s = starts_with(input, "spawnradius2")) ) {
      parse_int(s, &game->sr);
    }
    else if ( (s = starts_with(input, "ready")) ) {
      break;
    }
  };
  initialize_game(game);
  initialize_turn(game);
}

void output_move(Output *output, Ant *ant) {
  fprintf(output->outp, "o %d %d %c\n", ant->x, ant->y, ant->move);
}

void  output_end_turn(Output* output) {
  fprintf(output->outp, "go\n");
  fflush(output->outp);
}

void output_turn(Output *output, Game *game) {
  int index;
  Ant *ant;
  for(index=0;index<game->ant_tail;++index) {
    ant = game->ants + index;
    if ( ant->player == 0 ) {
      output_move(output, ant);
    }
  }
  output_end_turn(output);
}

void play_turn(Game *game) {
  for(int index=0;index<game->ant_tail;++index) {
    game->ants[index].move = NORTH;
  }
}

int main(int argc, char **argv) {
  Game   *game = new_game();
  Input  *input = new_input(stdin);
  Output *output = new_output(stdout);
  parse_game_description(input, game);
  output_end_turn(output);
  while ( parse_game_turn(input, game) ) {
    play_turn(game);
    output_turn(output, game);
    initialize_turn(game);
  }
}
