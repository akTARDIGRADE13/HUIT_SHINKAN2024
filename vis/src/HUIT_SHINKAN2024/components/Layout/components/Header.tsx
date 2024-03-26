import { type FC } from 'react';
import styles from './Header.module.css';

const title = import.meta.env.VITE_APP_TITLE;

const Header: FC = () => {
  return (
    <header className={styles.header}>
      <h1 className={styles.title}>{title}</h1>
    </header>
  );
};

export default Header;
