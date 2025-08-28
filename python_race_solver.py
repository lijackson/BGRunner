
# Online Python - IDE, Editor, Compiler, Interpreter

dice_probability = {}#{(1,1): 1./36, (1,2): 2./36, (1,3): 2./36, (1,4): 2./36, (1,5): 2./36, (1,6): 2./36, (2, 2): 1./36, (2,3): 2./36, (2,4): 2./36, (2,5): 2./36, (2,6): 2./36, (3,3): 1./36, (3,4): 2./36, (3,4): 2./36, (3,5): 2./36, (3,6): 2./36, (4,4): 1./36, (4,5): 2./36, (4,6): 2./36, (5,5): 1./36, (5,6): 2./36, (6,6): 1./36}

for a in range(1,7):
    for b in range(1,7):
        dice_probability[(a,b)] = 1./36

def get_all_next_boards(initial_board, roll):
    boards = [initial_board]
    if roll[0] == roll[1]:
        roll = (roll[0], roll[0], roll[0], roll[0])
    for d in roll:
        new_boards = []
        for board in boards:
            overshoot_index = max((i for i, x in enumerate(board) if x > 0), default=-1)
            no_moves = True
            for i in range(len(board)):
                if board[i] == 0:
                    continue
                if i-d < 0 and i != overshoot_index:
                    continue
                no_moves = False
                new_boards.append(move_new_board(board, (i,d)))
            if no_moves:
                new_boards.append(board)
        boards = new_boards
    return set(boards)

def move_new_board(board, move):
    return tuple(board[i] - (move[0]==i) + (move[0]-move[1]==i) for i in range(len(board)))

def add_scores(scores1, scores2, ratio):
    for s in scores2.keys():
        if s+1 not in scores1:
            scores1[s+1] = 0
        scores1[s+1] += scores2[s]*ratio

class Solver:
    def __init__(self):
        self.solved = {(0,0,0,0,0,0): {0: 1.0}}
        
    def solve(self, board):
        if board in self.solved:
            return self.solved[board]
        
        prob_distribution = {}
        for roll in dice_probability.keys():
            boards = get_all_next_boards(board, roll)
            for new_board in boards:
                sub_scores = self.solve(new_board)
                add_scores(prob_distribution, sub_scores, dice_probability[roll])
        
        self.solved[board] = prob_distribution
        print(board, prob_distribution)
        return prob_distribution

def main():
    s = Solver()
    
    test_board = (1,0,1,0,0,0) # WRONG
    ans = s.solve(test_board)
    print("\nactual output:")
    print(ans)


if __name__ == "__main__":
    main()