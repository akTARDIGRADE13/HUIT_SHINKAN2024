import { type FC } from 'react';
import Footer from './components/Footer';
import Header from './components/Header';
import styles from './Layout.module.css';

type LayoutProps = {
  children: React.ReactNode;
};

const Layout: FC<LayoutProps> = (props) => {
  return (
    <div className={styles['layout-wrapper']}>
      <Header />
      <main>{props.children}</main>
      <Footer />
    </div>
  );
};

export default Layout;
