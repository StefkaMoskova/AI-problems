import java.util.ArrayList;
import java.util.List;

public class CellsManipulator {

    private List<Cell> cells;

    public CellsManipulator(List<Cell> cells) {
        this.cells = cells;
        this.createRelationships();
    }

    public boolean isWinning() {
        for (Cell cell : this.cells) {
            if (!cell.hasCorrectValue()) {
                return false;
            }
        }
        return true;
    }

    public int calculateTotalManhattanDistance() {
        int sum = 0;
        for (Cell cell : this.cells) {
            Cell cellWithRequiredValue = findCellWithGivenValue(cell.getCurrentValue());
            if (cell.getCurrentValue() == 0) {
                continue;
            }
            sum += Math.abs(cell.getRowIndex() - cellWithRequiredValue.getRowIndex()) +
                    Math.abs(cell.getColIndex() - cellWithRequiredValue.getColIndex());
        }
        return sum;
    }

    public List<Cell> getCellsCopy() {
        List<Cell> copiedCells = new ArrayList<>();
        for (Cell cell : this.cells) {
            Cell newCell = new Cell(cell.getRowIndex(), cell.getColIndex(),
                    cell.getCurrentValue(), cell.getRequiredValue(),
                    cell.getCountOfCells());
            copiedCells.add(newCell);
        }
        return copiedCells;
    }

    public Cell getBlankCell() {
        for (Cell cell : this.cells) {
            if (cell.getCurrentValue() == 0) {
                return cell;
            }
        }
        return null;
    }

    public void move(String direction) {
        Cell blankCell = getBlankCell();
        Cell cellToSwap = null;
        switch (direction) {
            case "left": {
                cellToSwap = blankCell.getRightCell();
                break;
            }
            case "up": {
                cellToSwap = blankCell.getDownCell();
                break;
            }
            case "down": {
                cellToSwap = blankCell.getUpCell();
                break;
            }
            case "right": {
                cellToSwap = blankCell.getLeftCell();
                break;
            }
        }
        if (cellToSwap != null) {
            int firstValue = blankCell.getCurrentValue();
            int secondValue = cellToSwap.getCurrentValue();
            blankCell.setCurrentValue(secondValue);
            cellToSwap.setCurrentValue(firstValue);
        }
    }

    private void createRelationships() {
        for (Cell cell : this.cells) {
            Cell upCell = findCellByCoordinates(cell.getRowIndex() - 1, cell.getColIndex());
            cell.setUpCell(upCell);
            Cell downCell = findCellByCoordinates(cell.getRowIndex() + 1, cell.getColIndex());
            cell.setDownCell(downCell);
            Cell leftCell = findCellByCoordinates(cell.getRowIndex(), cell.getColIndex() - 1);
            cell.setLeftCell(leftCell);
            Cell rightCell = findCellByCoordinates(cell.getRowIndex(), cell.getColIndex() + 1);
            cell.setRightCell(rightCell);
        }
    }

    private Cell findCellWithGivenValue(int value) {
        for (Cell cell : this.cells) {
            if (cell.getRequiredValue() == value) {
                return cell;
            }
        }
        return null;
    }

    private Cell findCellByCoordinates(int rowIndex, int colIndex) {
        for (Cell cell : this.cells) {
            if (cell.getRowIndex() == rowIndex && cell.getColIndex() == colIndex) {
                return cell;
            }
        }
        return null;
    }


}
