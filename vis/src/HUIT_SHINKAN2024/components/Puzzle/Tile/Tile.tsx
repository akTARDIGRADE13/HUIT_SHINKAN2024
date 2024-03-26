// Tileコンポーネント
import { FC, CSSProperties } from 'react';
import styles from './Tile.module.css';

interface TileProps {
  number: number;
  backgroundColor?: 'correct' | 'white' | 'gradient'; // 新しいprops: backgroundColorを追加
  gradientValue?: number; // グラデーションの値
  style?: CSSProperties; // style プロパティを追加
}

const Tile: FC<TileProps> = ({
  number,
  backgroundColor = 'white',
  gradientValue,
  style,
}) => {
  const tileStyle: CSSProperties = {
    backgroundColor:
      number === 0
        ? 'var(--gray-25)' // numberが0の場合はvar(--gray-25)に
        : backgroundColor === 'correct'
          ? '#39ff14' // 'correct' の場合は真っ赤
          : backgroundColor === 'gradient' && typeof gradientValue === 'number'
            ? `rgba(255, 0, 0, ${gradientValue})`
            : backgroundColor === 'white'
              ? '#ffffff' // 'white' の場合は白に
              : backgroundColor, // その他の場合は指定された色を使用
    ...style, // 追加のスタイルをマージ
  };

  return (
    <div className={styles.tile} style={tileStyle}>
      {number}
    </div>
  );
};

export default Tile;
