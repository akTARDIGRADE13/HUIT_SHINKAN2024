import { type FC } from 'react';
import styles from './NotFound.module.css';

const NotFound: FC = () => {
  return (
    <div className={styles.container}>
      <h1 className={styles.title}>404 Not Found</h1>
      <p className={styles.message}>
        お探しのページが見つかりませんでした。
        <br />
        URLが間違っているか、削除された可能性があります。
      </p>
    </div>
  );
};

export default NotFound;
