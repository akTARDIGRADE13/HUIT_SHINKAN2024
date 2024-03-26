import { useState, useEffect, useCallback } from 'react';
import { ChangeEvent } from 'react';
import MarkdownContent from 'HUIT_SHINKAN2024/components/MarkdownContent/MarkdownContent';
import PlayButton from './components/PlayButton';
import SelectBox from './components/SelectBox';
import Slider from './components/Slider';
import styles from './BaseVisualizer.module.css';
import { useVisualizerInfo } from 'HUIT_SHINKAN2024/components/BaseVisualizer/context/useVisualizerInfo';

interface BaseVisualizerProps {
  children: React.ReactNode;
  // drawSvg: (fileContent: string) => string[];
}

const BaseVisualizer: React.FC<BaseVisualizerProps> = ({ children }) => {
  const [isPlaying, setIsPlaying] = useState(false);
  const [speed, setSpeed] = useState(100); // 再生速度（フレーム/秒）
  const [maxFrame, setMaxFrame] = useState(0);
  const {
    testCase,
    setTestCase,
    mode,
    setMode,
    currentFrame,
    setCurrentFrame,
    fileContent,
    setFileContent,
  } = useVisualizerInfo();

  const handleFileChange = useCallback(
    (e: ChangeEvent<HTMLTextAreaElement>) => {
      const file = e.target.value;
      if (!file) {
        setFileContent('');
        setMaxFrame(0);
        setCurrentFrame(0);
      } else {
        setFileContent(file);
        setMaxFrame(file.length);
        setCurrentFrame(file.length);
      }
    },
    [setFileContent, setMaxFrame, setCurrentFrame],
  );

  useEffect(() => {
    const handleKeyDown = (event: KeyboardEvent) => {
      if (mode === 'manual') {
        const { key } = event;
        const validKeys = ['ArrowUp', 'ArrowDown', 'ArrowLeft', 'ArrowRight'];
        if (validKeys.includes(key)) {
          event.preventDefault();
          let newContent = fileContent;
          const lastChar = newContent.charAt(newContent.length - 1);
          const newPosition = {
            ArrowUp: 'U',
            ArrowDown: 'D',
            ArrowLeft: 'L',
            ArrowRight: 'R',
          }[key];

          // 指定された条件に応じて末尾を削除する
          if (
            (lastChar === 'L' && key === 'ArrowRight') ||
            (lastChar === 'R' && key === 'ArrowLeft') ||
            (lastChar === 'U' && key === 'ArrowDown') ||
            (lastChar === 'D' && key === 'ArrowUp')
          ) {
            newContent = newContent.slice(0, -1);
          } else {
            newContent += newPosition;
          }

          // 更新後の値をセット
          setFileContent(newContent);
          setMaxFrame(newContent.length);
          setCurrentFrame(newContent.length);
        }
      }
    };

    window.addEventListener('keydown', handleKeyDown);
    return () => {
      window.removeEventListener('keydown', handleKeyDown);
    };
  }, [fileContent, mode, setFileContent, setMaxFrame, setCurrentFrame]);

  // useEffectを使ってfileContentの変更を監視し、textareaに反映
  useEffect(() => {
    const textarea = document.getElementById('textArea') as HTMLTextAreaElement;
    if (textarea) {
      textarea.value = fileContent;
    }
  }, [fileContent]);

  return (
    <div className={styles.container}>
      <div className={styles['setting-container']}>
        <div className={styles['bottans-container']}>
          <textarea
            id="textArea"
            className={styles.textbox}
            placeholder="Enter text here..."
            onChange={(e) => handleFileChange(e)}
            // textareaの値をfileContentにバインド
            value={fileContent}
          />
        </div>
        <div className={styles['sliders-container']}>
          <Slider
            label="Frame"
            min={0}
            max={maxFrame} // ここはファイル読み込んで値を決定
            value={currentFrame}
            onChange={setCurrentFrame}
          />
          <Slider
            label="Speed"
            min={1}
            max={300}
            value={speed}
            onChange={setSpeed}
          />
          <PlayButton
            isPlaying={isPlaying}
            onClick={() => setIsPlaying(!isPlaying)}
          />
        </div>
        <div className={styles['sliders-container']}>
          <SelectBox
            label="testcase"
            options={Array.from({ length: 3 }, (_, index) => index + 1)} // 例えば、適切な範囲を指定
            value={testCase}
            onChange={(value) => {
              setTestCase(value as number);
            }}
          />
          <SelectBox
            label="mode"
            options={['normal', 'color1', 'color2', 'manual']} // 例えば、適切な範囲を指定
            value={mode}
            onChange={(value) => {
              setMode(value as string);
            }}
          />
        </div>
      </div>
      <div className={styles['svg-wrapper']}>{children}</div>
      <MarkdownContent mdPath={`/md/howto.md`} />
    </div>
  );
};

export default BaseVisualizer;
