import React from 'react';
import styles from './PlayButton.module.css';

interface PlayButtonProps {
  isPlaying: boolean;
  onClick: () => void;
}

const PlayButton: React.FC<PlayButtonProps> = ({ isPlaying, onClick }) => {
  return (
    <p>
      <input
        type="button"
        value={isPlaying ? 'stop' : 'start'}
        className={styles['play-button']}
        onClick={onClick}
      />
    </p>
  );
};

export default PlayButton;
