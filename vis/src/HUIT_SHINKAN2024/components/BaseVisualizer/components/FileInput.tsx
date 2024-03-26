import React, { useState } from 'react';
import styles from './FileInput.module.css';

interface FileInputProps {
  onFileChange: (files: FileList | null) => void;
}

const FileInput: React.FC<FileInputProps> = ({ onFileChange }) => {
  const [fileName, setFileName] = useState<string | null>(null);

  const handleFileChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    const files = event.target.files;
    onFileChange(files);
    setFileName(files?.[0]?.name || null);
  };

  return (
    <form>
      <input
        type="file"
        name="file"
        id="file"
        className={styles['file-input']}
        multiple
        onChange={handleFileChange}
      />
      <label htmlFor="file" className={styles['file-label']}>
        {fileName ? fileName : 'ファイルを選択'}
      </label>
    </form>
  );
};

export default FileInput;
