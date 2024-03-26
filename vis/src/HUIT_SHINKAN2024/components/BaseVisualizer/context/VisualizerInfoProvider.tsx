import type React from 'react';
import { type ReactNode, type Dispatch, type SetStateAction } from 'react';
import { createContext, useState } from 'react';

export interface VisualizerInfoContextProps {
  testCase: number;
  setTestCase: Dispatch<SetStateAction<number>>;
  mode: string;
  setMode: Dispatch<SetStateAction<string>>;
  currentFrame: number;
  setCurrentFrame: Dispatch<SetStateAction<number>>;
  fileContent: string;
  setFileContent: Dispatch<SetStateAction<string>>;
}

export const VisualizerInfoContext = createContext<VisualizerInfoContextProps>({
  testCase: 1,
  setTestCase: () => {},
  mode: 'normal',
  setMode: () => {},
  currentFrame: 0,
  setCurrentFrame: () => {},
  fileContent: '',
  setFileContent: () => {},
});

export const VisualizerInfoProvider: React.FC<{ children: ReactNode }> = ({
  children,
}) => {
  const [testCase, setTestCase] = useState<number>(1);
  const [mode, setMode] = useState<string>('normal');
  const [currentFrame, setCurrentFrame] = useState<number>(0);
  const [fileContent, setFileContent] = useState<string>('');

  return (
    <VisualizerInfoContext.Provider
      value={{
        testCase,
        setTestCase,
        mode,
        setMode,
        currentFrame,
        setCurrentFrame,
        fileContent,
        setFileContent,
      }}
    >
      {children}
    </VisualizerInfoContext.Provider>
  );
};
