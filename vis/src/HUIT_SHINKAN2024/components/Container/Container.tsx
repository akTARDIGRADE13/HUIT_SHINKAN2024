import { type FC } from 'react';
import styles from './Container.module.css';

type ContainerProps = {
  children: React.ReactNode;
  large?: boolean;
};

const Container: FC<ContainerProps> = ({ children, large = false }) => {
  return (
    <div className={large ? styles.large : styles.default}>{children}</div>
  );
};

export default Container;
