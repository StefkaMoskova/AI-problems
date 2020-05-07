import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class IDA {

    private Node startingNode;
    private List<String> moves;

    public IDA(List<Cell> cells) {
        this.startingNode = new Node(cells);
        this.moves = new ArrayList<>();
    }

    public List<String> winningMoves() {
        int threshold = this.startingNode.getCost();
        Node node = this.startingNode;
        while (!node.isSolution()) {
            node = idaStar(startingNode, 0, threshold);
            if (!node.isSolution()) {
                threshold++;
            }
        }
        List<String> reversedResult = createMoves(node);
        Collections.reverse(reversedResult);
        return reversedResult;
    }

    public Node idaStar(Node node, int cost, int threshold) 
    {
        int estimatedCost = node.getCost() + cost;
        
        if (node.isSolution() || estimatedCost > threshold) 
        {
            node.setEstimatedMinimumCost(cost);
            return node;
        }
        
        int minimalCost = Integer.MAX_VALUE;
        
        Cell blankCell = node.getCellsManipulator().getBlankCell();
        
        for (String move : getPossibleMovesWithoutReversals(node, blankCell)) 
        {
            Node childNode = getChildOfNodeAndMove(node, move);
            Node childIda = idaStar(childNode, cost + 1, threshold);
           
            if (childIda.isSolution())
            {
                return childIda;
            }
            
            if (childIda.getEstimatedMinimumCost() < minimalCost) 
            {
                minimalCost = childIda.getEstimatedMinimumCost();
            }
        }
        
        node.setEstimatedMinimumCost(minimalCost);
        
        return node;
    }

    private List<String> createMoves(Node node) {
        if (node == null || node.getPreviousNode() == null) {
            return  this.moves;
        }
        String previousMove = node.getPreviousMove();
        Node previousNode = node.getPreviousNode();
        this.moves.add(node.getPreviousMove());
        return createMoves(previousNode);
    }

    private List<String> getPossibleMovesWithoutReversals(Node node, Cell blankCell) {
        List<String> possibleDirections = blankCell.getPossibleDirections();
        String previousMove = node.getPreviousMove();
        if (previousMove == null) {
            return possibleDirections;
        }
        String reverseMove = null;
        switch (previousMove) {
            case "left": {
                reverseMove = "right";
                break;
            }
            case "up": {
                reverseMove = "down";
                break;
            }
            case "down": {
                reverseMove = "up";
                break;
            }
            case "right": {
                reverseMove = "left";
                break;
            }
        }
        List<String> remainingDirections = new ArrayList<>();
        for (String direction : possibleDirections) {
            if (!direction.equals(reverseMove)) {
                remainingDirections.add(direction);
            }
        }
        return remainingDirections;
    }

    private Node getChildOfNodeAndMove(Node node, String direction) {
        List<Cell> newCells = node.getCellsManipulator().getCellsCopy();
        Node childNode = new Node(newCells);
        childNode.getCellsManipulator().move(direction);
        childNode.changeCost();
        childNode.setPreviousMove(direction);
        childNode.setPreviousNode(node);
        return childNode;
    }
}
