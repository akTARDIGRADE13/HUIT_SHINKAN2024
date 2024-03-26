import { FC } from 'react';
import Layout from 'HUIT_SHINKAN2024/components/Layout/Layout';
import Container from 'HUIT_SHINKAN2024/components/Container/Container';
import BaseVisualizer from 'HUIT_SHINKAN2024/components/BaseVisualizer/BaseVisualizer';
import Puzzle from 'HUIT_SHINKAN2024/components/Puzzle/Puzzle';
import testCase1 from 'HUIT_SHINKAN2024/testcase/testcase1';
import testCase2 from 'HUIT_SHINKAN2024/testcase/testcase2';
import testCase3 from 'HUIT_SHINKAN2024/testcase/testcase3';
import { useVisualizerInfo } from 'HUIT_SHINKAN2024/components/BaseVisualizer/context/useVisualizerInfo';

const Home: FC = () => {
  const { mode, currentFrame, testCase, fileContent } = useVisualizerInfo();
  console.log(mode, currentFrame);

  const boards: number[][][] = [testCase1, testCase2, testCase3];
  const puzzleBoard: number[][] = JSON.parse(
    JSON.stringify(boards[testCase - 1]),
  );

  let x = 0,
    y = 0;
  puzzleBoard.forEach((row, idx) => {
    const index = row.findIndex((element) => element === 0);
    if (index !== -1) {
      x = idx;
      y = index;
    }
  });

  let err = 0; // エラーがある場合、1以上の値が入る変数
  const dx: number[] = [0, 0, 1, -1],
    dy: number[] = [1, -1, 0, 0];

  fileContent.split('').forEach((char, idx) => {
    let k = -1;
    if (char === 'R') k = 0;
    else if (char === 'L') k = 1;
    else if (char === 'D') k = 2;
    else if (char === 'U') k = 3;
    if (k === -1) {
      err = idx + 1;
      return;
    }
    const nx = x + dx[k];
    const ny = y + dy[k];
    if (Math.min(nx, ny) < 0 || Math.max(nx, ny) >= puzzleBoard.length) {
      err = idx + 1;
      return;
    }
    [puzzleBoard[x][y], puzzleBoard[nx][ny]] = [
      puzzleBoard[nx][ny],
      puzzleBoard[x][y],
    ];
    x = nx;
    y = ny;
  });

  let headerText = '';
  let cost = 0;
  if (mode === 'normal' || mode === 'manual') {
    headerText = 'Total Cost:';
    cost = fileContent.length;
  } else if (mode === 'color1') {
    headerText = 'Correct:';
    puzzleBoard.forEach((row, idx) => {
      row.forEach((num, idx2) => {
        if (
          idx === (((num - 1) / puzzleBoard.length) | 0) &&
          idx2 === (num - 1) % puzzleBoard.length
        ) {
          cost += 1;
        }
      });
    });
  } else {
    headerText = 'Manhattan Cost:';
    puzzleBoard.forEach((row, idx) => {
      row.forEach((num, idx2) => {
        if (num === 0) return;
        cost +=
          Math.abs(idx - (((num - 1) / puzzleBoard.length) | 0)) +
          Math.abs(idx2 - ((num - 1) % puzzleBoard.length));
      });
    });
  }

  return (
    <Layout>
      <Container>
        <BaseVisualizer>
          {err >= 1 ? (
            <div>{`${err}文字目の入力に誤りがあります`}</div>
          ) : (
            <>
              <h2>
                {headerText} {cost}
              </h2>
              <Puzzle board={puzzleBoard} mode={mode} />
            </>
          )}
        </BaseVisualizer>
      </Container>
    </Layout>
  );
};

export default Home;
