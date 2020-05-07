import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Main {

    public static void main(String[] args) throws IOException {
        BufferedReader bfr = new BufferedReader(new InputStreamReader(System.in));
        System.out.println("N: ");
	    int countOfCells = (Integer.parseInt(bfr.readLine())) + 1;
        System.out.println("Zero index: ");
        int requiredPositionOfZero = Integer.parseInt(bfr.readLine());
        if (requiredPositionOfZero == -1) {
            requiredPositionOfZero = countOfCells - 1;
        }
        System.out.println("Now enter the numbers in the puzzle");
        List<Cell> cells = new ArrayList<>();
        int counter = 1;
        int index = -1;
        for (int row = 0; row < Math.sqrt(countOfCells); row++) {
            int[] numbers = Arrays.stream(bfr.readLine().split("\\s+"))
                    .mapToInt(Integer::parseInt)
                    .toArray();
            for (int col = 0; col < Math.sqrt(countOfCells); col++) {
                if (index + 1 == requiredPositionOfZero) {
                    cells.add(index + 1, new Cell(row, col, numbers[col], 0, countOfCells));
                } else {
                    cells.add(index + 1, new Cell(row, col, numbers[col], counter, countOfCells));
                    counter++;
                }
                index++;
            }
        }
        IDA ida = new IDA(cells);
        List<String> solution = ida.winningMoves();
        System.out.println("The count of the moves to order the puzzle is: " + solution.size());
        System.out.println("The steps to solve the puzzle are:");
        solution.forEach(System.out::println);
    }

}
