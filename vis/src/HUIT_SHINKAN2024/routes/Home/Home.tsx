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
  const puzzleBoard: number[][] = boards[testCase - 1];
  let x = 0,
    y = 0;
  for (let i = 0; i < puzzleBoard.length; i++) {
    const index = puzzleBoard[i].findIndex((element) => element === 0);
    if (index !== -1) {
      (x = i), (y = index); // 要素が見つかった場合、行と列のインデックスを返す
    }
  }
  let err = 0; // エラーがある場合、1以上の値が入る変数
  const dx: number[] = [0, 0, 1, -1],
    dy: number[] = [1, -1, 0, 0];
  for (const [idx, char] of fileContent.split('').entries()) {
    let k = -1;
    if (char === 'R') {
      k = 0;
    }
    if (char === 'L') {
      k = 1;
    }
    if (char === 'D') {
      k = 2;
    }
    if (char === 'U') {
      k = 3;
    }
    if (k === -1) {
      err = idx + 1;
      break;
    }
    const nx = x + dx[k],
      ny = y + dy[k];
    if (Math.min(nx, ny) < 0 || Math.max(nx, ny) >= puzzleBoard.length) {
      err = idx + 1;
      break;
    }
    const tmp = puzzleBoard[nx][ny];
    puzzleBoard[nx][ny] = puzzleBoard[x][y];
    puzzleBoard[x][y] = tmp;
    (x = nx), (y = ny);
  }

  return (
    <Layout>
      <Container>
        <BaseVisualizer>
          {err >= 1 ? (
            <div>{`${err}文字目の入力に誤りがあります`}</div>
          ) : (
            <Puzzle board={puzzleBoard} mode={mode} />
          )}
        </BaseVisualizer>
      </Container>
    </Layout>
  );
};

export default Home;
