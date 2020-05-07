import java.util.Arrays;
import java.util.List;

public class Node {

    private List<Cell> cells;
    private CellsManipulator cellsManipulator;
    private String previousMove;
    private Node previousNode;
    private int estimatedMinimumCost;
    private int cost;

    public Node(List<Cell> cells) {
        this.cells = cells;
        this.cellsManipulator = new CellsManipulator(this.cells);
        this.estimatedMinimumCost = Integer.MAX_VALUE;
        this.cost = this.cellsManipulator.calculateTotalManhattanDistance();
    }

    @Override
    public int hashCode() {
        Cell[] cells = this.cells.toArray(new Cell[this.cells.size()]);
        return Arrays.deepHashCode(cells);
    }

    @Override
    public boolean equals(Object obj) {
        if (!(obj instanceof Node)) {
            return false;
        }
        if (obj == this) {
            return true;
        }
        Cell[] cells = this.cells.toArray(new Cell[this.cells.size()]);
        Cell[] objCells = ((Node) obj).cells.toArray(new Cell[((Node) obj).cells.size()]);
        return Arrays.deepEquals(cells, objCells);
    }

    public String getPreviousMove() {
        return this.previousMove;
    }

    public Node getPreviousNode() {
        return this.previousNode;
    }

    public int getCost() {
        return this.cost;
    }

    public void setPreviousMove(String previousMove) {
        this.previousMove = previousMove;
    }

    public void setPreviousNode(Node previousNode) {
        this.previousNode = previousNode;
    }

    public CellsManipulator getCellsManipulator() {
        return this.cellsManipulator;
    }

    public int getEstimatedMinimumCost() {
        return this.estimatedMinimumCost;
    }

    public void setEstimatedMinimumCost(int estimatedMinimumCost) {
        this.estimatedMinimumCost = estimatedMinimumCost;
    }

    public boolean isSolution() {
        return this.cellsManipulator.isWinning();
    }

    public void changeCost() {
        this.cost = this.cellsManipulator.calculateTotalManhattanDistance();
    }

}
