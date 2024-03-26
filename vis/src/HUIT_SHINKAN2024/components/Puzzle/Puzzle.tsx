import React from 'react';
import Tile from './Tile/Tile';
import styles from './Puzzle.module.css';

interface PuzzleProps {
  board: number[][];
  mode: string;
}

const Puzzle: React.FC<PuzzleProps> = ({ board, mode }) => {
  const tileSize = 500 / board.length;

  const getTileProps = (number: number, x: number, y: number) => {
    if (
      mode === 'color1' &&
      x === ((number / board.length) | 0) &&
      y === number % board.length
    ) {
      return { backgroundColor: 'correct' as const };
    } else if (mode === 'color2') {
      const manhattanDistance =
        Math.abs(x - ((number / board.length) | 0)) +
        Math.abs(y - (number % board.length));
      return {
        backgroundColor: 'gradient' as const,
        gradientValue: manhattanDistance / (board.length * 2 - 2),
      };
    }
    return {};
  };

  return (
    <div
      className={styles.puzzle}
      style={{
        width: 500,
        height: 500,
      }}
    >
      {board.map((row, rowIndex) => (
        <div key={rowIndex} className={styles.row}>
          {row.map((number, columnIndex) => (
            <Tile
              key={`${rowIndex}-${columnIndex}`}
              number={number}
              style={{ width: tileSize, height: tileSize }}
              {...getTileProps(number - 1, rowIndex, columnIndex)}
            />
          ))}
        </div>
      ))}
    </div>
  );
};

export default Puzzle;
