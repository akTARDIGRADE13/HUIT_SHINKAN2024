import { useContext } from 'react';
import {
  VisualizerInfoContext,
  type VisualizerInfoContextProps,
} from './VisualizerInfoProvider';

export const useVisualizerInfo = (): VisualizerInfoContextProps => {
  const context = useContext(VisualizerInfoContext);
  if (!context) {
    throw new Error(
      'useVisualizerInfo must be used within a VisualizerInfoProvider',
    );
  }

  return context;
};
