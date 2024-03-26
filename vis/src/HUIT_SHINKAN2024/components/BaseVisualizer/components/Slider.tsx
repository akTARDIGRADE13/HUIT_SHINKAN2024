import React from 'react';
import styles from './Slider.module.css';

interface SliderProps {
  label: string;
  min: number;
  max: number;
  value: number;
  onChange: (value: number) => void;
}

const Slider: React.FC<SliderProps> = ({
  label,
  min,
  max,
  value,
  onChange,
}) => {
  return (
    <div className={styles['slider-container']}>
      <input
        type="range"
        min={min}
        max={max}
        value={value}
        className={styles.slider}
        onChange={(e) => onChange(Number(e.target.value))}
      />
      <label className={styles['slider-label']}>
        {label}
        <span className={styles['slider-value']}>{value}</span>
      </label>
    </div>
  );
};

export default Slider;
