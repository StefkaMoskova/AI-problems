import java.util.ArrayList;
import java.util.List;

public class Cell {

    private int rowIndex;
    private int colIndex;
    
    private int currentValue;
    private int requiredValue;
    
    private Cell upCell;
    private Cell downCell;
    private Cell leftCell;
    private Cell rightCell;
    
    private List<String> possibleDirections;
    
    private int countOfCells;

    public Cell(int rowIndex, int colIndex, int currentValue, int requiredValue, int countOfCells) {
        this.rowIndex = rowIndex;
        this.colIndex = colIndex;
        this.currentValue = currentValue;
        this.requiredValue = requiredValue;
        this.countOfCells = countOfCells;
        this.possibleDirections = initializePossibleDirections();
    }

    public int getCurrentValue() {
        return this.currentValue;
    }

    public int getRequiredValue() {
        return this.requiredValue;
    }

    public int getRowIndex() {
        return this.rowIndex;
    }

    public int getColIndex() {
        return this.colIndex;
    }

    public Cell getUpCell() {
        return this.upCell;
    }

    public Cell getDownCell() {
        return this.downCell;
    }

    public Cell getLeftCell() {
        return this.leftCell;
    }

    public Cell getRightCell() {
        return this.rightCell;
    }

    public List<String> getPossibleDirections() {
        return this.possibleDirections;
    }

    public int getCountOfCells() {
        return countOfCells;
    }

    public void setUpCell(Cell upCell) {
        this.upCell = upCell;
    }

    public void setDownCell(Cell downCell) {
        this.downCell = downCell;
    }

    public void setLeftCell(Cell leftCell) {
        this.leftCell = leftCell;
    }

    public void setRightCell(Cell rightCell) {
        this.rightCell = rightCell;
    }

    public void setCurrentValue(int currentValue) {
        this.currentValue = currentValue;
    }

    public boolean hasCorrectValue() {
        return this.currentValue == this.requiredValue;
    }

    private List<String> initializePossibleDirections() {
        List<String> directions = new ArrayList<>();
        if (this.colIndex + 1 < Math.sqrt(countOfCells)) {
            directions.add("left");
        }
        if (this.rowIndex + 1 < Math.sqrt(countOfCells)) {
            directions.add("up");
        }
        if (this.rowIndex - 1 >= 0) {
            directions.add("down");
        }
        if (this.colIndex - 1 >= 0) {
            directions.add("right");
        }
        return directions;
    }
}
