from engines import random1, random2, random3, student
import othello
from othello import *
import multiprocessing
from collections import Counter

def game_quiet(white_engine, black_engine, game_time=300.0, verbose=False):
    """ Run a single game. Raise RuntimeError in the event of time expiration.
    Raise LookupError in the case of a bad move. The tournament engine must
    handle these exceptions. """

    # Initialize variables
    board = Board()
    time = { -1 : game_time, 1 : game_time }
    engine = { -1 : black_engine, 1 : white_engine }

    if verbose:
        print "INITIAL BOARD\n--\n"
        board.display(time)
    
    # Do rounds
    for move_num in range(60):
        moves = []
        for color in [-1, 1]:
            start_time = timeit.default_timer()
            move = get_move(board, engine[color], color, move_num, time)
            end_time = timeit.default_timer()
            # Update user time
            time[color] -= round(end_time - start_time, 1) 
            
            if time[color] < 0:
                raise RuntimeError(color)

            # Make a move, otherwise pass
            if move is not None:
                board.execute_move(move, color)
                moves.append(move)

                if verbose:
                    print "--\n"
                    print "Round " + str(move_num + 1) + ": " + player[color] + " plays in " + move_string(move) + '\n'
                    board.display(time)

        if not moves:
            # No more legal moves. Game is over.
            break
    return board

if __name__ == '__main__':

    alpha_beta_flag = False
    if len(sys.argv) == 1:
        NUM_GAMES = 100
    elif len(sys.argv) == 2: 
        NUM_GAMES = int(sys.argv[1])
    elif len(sys.argv) == 3:
        for arg in sys.argv:
            try:
                NUM_GAMES = int(arg)
            except:
                alpha_beta_flag = True if str(arg) == "-ab" else False

    white_engine = "student"
    black_engine = "random2"
    engines_b = __import__('engines.' + black_engine)
    engines_w = __import__('engines.' + white_engine)

    engine_b = engines_b.__dict__[black_engine].__dict__['engine']()
    engine_w = engines_w.__dict__[white_engine].__dict__['engine']()
        
    # engine_b.alpha_beta = False
    othello.player[-1] = black_engine + " (black)"
    othello.player[1] = white_engine + " (white)"

    engine_w.alpha_beta = alpha_beta_flag
    
    # print black_engine + " vs. " + white_engine + "\n"
    print "using %d cpus" % multiprocessing.cpu_count()

    def f(x):
        board = game_quiet(engine_w, engine_b, 60, verbose = False)
        stats = othello.winner(board)
        print stats
        if stats[0] == 1:
            return "win"
        elif stats[0] == 0:
            return "tie"
        return "loss"

    p = multiprocessing.Pool( multiprocessing.cpu_count() )
    outcome = p.map(f, range(NUM_GAMES))

    counts = Counter(outcome)
    
    print counts.items()
