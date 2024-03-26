// src/components/SelectBox.tsx
import React, { ChangeEvent } from 'react';
import styles from './SelectBox.module.css'; // 新しく追加

interface SelectBoxProps {
  label: string;
  options: string[] | number[];
  value: string | number;
  onChange: (value: string | number) => void;
}

const SelectBox: React.FC<SelectBoxProps> = ({
  label,
  options,
  value,
  onChange,
}) => {
  const handleSelectChange = (event: ChangeEvent<HTMLSelectElement>) => {
    const selectedValue = event.target.value;
    onChange(selectedValue);
  };

  return (
    <div className={styles['select-container']}>
      <label className={styles.label}>{label}:</label>
      <select
        className={styles.select}
        value={value}
        onChange={handleSelectChange}
      >
        {options.map((option) => (
          <option key={option} value={option}>
            {option}
          </option>
        ))}
      </select>
    </div>
  );
};

export default SelectBox;
