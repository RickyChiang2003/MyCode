# multiAgents.py
# --------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from util import manhattanDistance
from game import Directions
import random, util

from game import Agent
from pacman import GameState

"************************************ YOUR CODE HERE ************************************"
MAXVALUE = 1000000000
MINVALUE = -1000000000
"************************************ YOUR CODE END *************************************"

class ReflexAgent(Agent):
    """
    A reflex agent chooses an action at each choice point by examining
    its alternatives via a state evaluation function.

    The code below is provided as a guide.  You are welcome to change
    it in any way you see fit, so long as you don't touch our method
    headers.
    """


    def getAction(self, gameState: GameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {NORTH, SOUTH, WEST, EAST, STOP}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState: GameState, action):
        """
        Design a better evaluation function here.
        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = currentGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates] 
        "************************************ YOUR CODE HERE ************************************"
        # init functions
        def init_closestFoodDis(newPos,newFoodList):
            if(len(newFoodList)) == 0:
                return 0
            else:
                return min([manhattanDistance(newPos, f) for f in newFoodList]) 
        def init_closestGhostDis_ScaredTime(newPos,newGhostStates,newScaredTimes):
            closestg = MAXVALUE
            Scaredt = 0
            for g,t in zip(newGhostStates,newScaredTimes):
                d = manhattanDistance(newPos, g.getPosition())
                if d < closestg:
                    closestg = d
                    Scaredt = t
            return closestg, Scaredt
        
        # main code 
        newFoodList = newFood.asList()
        closestFoodDis = init_closestFoodDis(newPos,newFoodList)
        closestGhostDis, ScaredTime = init_closestGhostDis_ScaredTime(newPos,newGhostStates,newScaredTimes)
        score = 0
         # find food
        if closestFoodDis <= 1:
            score += 300/(closestFoodDis+1)
        score -= closestFoodDis * 3
        # excape from ghost or go to ghost
        if closestGhostDis <= 1 and ScaredTime == 0:
            score -= 500
        if ScaredTime == 0:
            score -= 130/(closestGhostDis+1)

        # prevent from stop
        if action == 'Stop': score -= 200
        return score+successorGameState.getScore()    
        "************************************ YOUR CODE END *************************************"

def scoreEvaluationFunction(currentGameState: GameState):
    """
    This default evaluation function just returns the score of the state.
    The score is the same one displayed in the Pacman GUI.

    This evaluation function is meant for use with adversarial search agents
    (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
    This class provides some common elements to all of your
    multi-agent searchers.  Any methods defined here will be available
    to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

    You *do not* need to make any changes here, but you can if you want to
    add functionality to all your adversarial search agents.  Please do not
    remove anything, however.

    Note: this is an abstract class: one that should not be instantiated.  It's
    only partially specified, and designed to be extended.  Agent (game.py)
    is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
    Your minimax agent (question 2)
    """

    def getAction(self, gameState: GameState):
        """
        Returns the minimax action from the current gameState using self.depth
        and self.evaluationFunction.

        Here are some method calls that might be useful when implementing minimax.

        gameState.getLegalActions(agentIndex):
        Returns a list of legal actions for an agent
        agentIndex=0 means Pacman, ghosts are >= 1

        gameState.generateSuccessor(agentIndex, action):
        Returns the successor game state after an agent takes an action

        gameState.getNumAgents():
        Returns the total number of agents in the game

        gameState.isWin():
        Returns whether or not the game state is a winning state

        gameState.isLose():
        Returns whether or not the game state is a losing state
        """
        "************************************ YOUR CODE HERE ************************************"
        nextaction, nextvalue = self.getValue(0,0,gameState)
        return nextaction
    
    def getValue(self, index, depth, gameState):
        # two return cases: TerminalState and MaxMinValue
        if len(gameState.getLegalActions(index))>0 and depth<self.depth:
            return self.MaxMinValue(index, depth, gameState)
        return "", self.evaluationFunction(gameState)
    
    def MaxMinValue(self, index, depth, gameState):
        # I prefer "next" to "successor"  ><
        nextaction = ""
        localMax = float(MINVALUE)
        localMin = float(MAXVALUE)
        legalMoves = gameState.getLegalActions(index)
        numAgent = gameState.getNumAgents()

        for action in legalMoves:
            next_index = (index+1 if index+1 != numAgent else 0      ) 
            next_depth = (depth   if index+1 != numAgent else depth+1)
            next_ = gameState.generateSuccessor(index,action)
            some_action, curr_value = self.getValue(next_index, next_depth, next_)
            
            # seperate Max,Min by index: index==0 if it's Max, else Min
            if index == 0 and curr_value > localMax:
                nextaction, localMax = action, curr_value
            if index != 0 and curr_value < localMin:
                nextaction, localMin = action, curr_value
        
        return nextaction, (localMax if index==0 else localMin)
    
    "************************************ YOUR CODE END *************************************"

class AlphaBetaAgent(MultiAgentSearchAgent):
    """
    Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState: GameState):
        """
        Returns the minimax action using self.depth and self.evaluationFunction
        """
        "************************************ YOUR CODE HERE ************************************"
        nextaction, nextvalue = self.getValue_ab(0,0,MINVALUE,MAXVALUE,gameState)
        return nextaction
    
    def getValue_ab(self, index, depth, alfa, beta, gameState):
        # two return cases: TerminalState and MaxMinValue
        if len(gameState.getLegalActions(index))>0 and depth<self.depth:
            return self.MaxMinValue_ab(index,depth,alfa,beta,gameState)
        return "", self.evaluationFunction(gameState)
    
    def MaxMinValue_ab(self, index, depth, alfa, beta, gameState):
        # I prefer "next" to "successor" (and btw sorry for "alfa")  ><
        nextaction = ""
        localMax = float(MINVALUE)
        localMin = float(MAXVALUE)
        legalMoves = gameState.getLegalActions(index)
        numAgent = gameState.getNumAgents()

        for action in legalMoves:
            next_index = (index+1 if index+1 != numAgent else 0      ) 
            next_depth = (depth   if index+1 != numAgent else depth+1)
            next_ = gameState.generateSuccessor(index,action)
            some_action, curr_value = self.getValue_ab(next_index,next_depth,alfa,beta,next_)
            
            # seperate Max,Min by index: index==0 if it's Max, else Min
            if index == 0:
                if curr_value > localMax:
                    nextaction, localMax = action, curr_value
                if localMax > beta:
                    return nextaction, localMax
                alfa = max(alfa,localMax)
            if index != 0:
                if curr_value < localMin:
                    nextaction, localMin = action, curr_value
                if localMin < alfa:
                    return nextaction, localMin
                beta = min(beta, localMin)
            
        return nextaction, (localMax if index==0 else localMin)
    "************************************ YOUR CODE END *************************************"

class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState: GameState):
        """
        Returns the expectimax action using self.depth and self.evaluationFunction

        All ghosts should be modeled as choosing uniformly at random from their
        legal moves.
        """
        "************************************ YOUR CODE HERE ************************************"
        nextaction, nextvalue = self.getValue(0,0,gameState)
        return nextaction
    
    def getValue(self, index, depth, gameState):
        # two return cases: TerminalState and MaxExpValue(Min-->Exp)
        if len(gameState.getLegalActions(index))>0 and depth<self.depth:
            return self.MaxExpValue(index, depth, gameState)
        #return "", gameState.getScore()
        return "", self.evaluationFunction(gameState)
    
    def MaxExpValue(self, index, depth, gameState):
        # I prefer "next" to "successor"  ><
        nextaction = ""
        localMax = float(MINVALUE)
        localExp = 0
        legalMoves = gameState.getLegalActions(index)
        numofMoves = len(legalMoves)
        numAgent = gameState.getNumAgents()

        for action in legalMoves:
            next_index = (index+1 if index+1 != numAgent else 0      ) 
            next_depth = (depth   if index+1 != numAgent else depth+1)
            next_ = gameState.generateSuccessor(index,action)
            some_action, curr_value = self.getValue(next_index, next_depth, next_)
            
            # seperate Max,Exp by index: index==0 if it's Max, else Exp
            if index == 0 and curr_value > localMax:
                nextaction, localMax = action, curr_value
            if index != 0:
                localExp += curr_value/numofMoves
        
        return nextaction, (localMax if index==0 else localExp)
    "************************************ YOUR CODE END *************************************"

def betterEvaluationFunction(currentGameState: GameState):
    """
    Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
    evaluation function (question 5).

    DESCRIPTION: <write something here so we know what you did>
    """
    "************************************ YOUR CODE HERE ************************************"
    def init_closestFoodDis(Pos,Food):
        if(len(Food)) == 0:
            return 0
        else:
            return min([manhattanDistance(Pos, f) for f in Food]) 
    Pos = currentGameState.getPacmanPosition()
    Food = currentGameState.getFood().asList()
    GhostPos = currentGameState.getGhostPositions()
    #closestGhostDis = init_closestGhostDis(Pos,GhostPos)
    closestFoodDis = init_closestFoodDis(Pos,Food)
    
    score = currentGameState.getScore()
    score += 20/(closestFoodDis+1)
    #score -= (5 if closestGhostDis <= 1 else 0)
    return score
    "************************************ YOUR CODE END *************************************"

# Abbreviation
better = betterEvaluationFunction
