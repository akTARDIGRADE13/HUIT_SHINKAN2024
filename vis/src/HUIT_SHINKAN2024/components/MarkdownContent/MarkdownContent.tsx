import { FC } from 'react';
import ReactMarkdown from 'react-markdown';
import rehypeKatex from 'rehype-katex';
import rehypeRaw from 'rehype-raw';
import rehypeSlug from 'rehype-slug';
import {
  remarkDefinitionList,
  defListHastHandlers,
} from 'remark-definition-list';
import remarkGfm from 'remark-gfm';
import remarkMath from 'remark-math';
import CodeBlock from './components/CodeBlock'; // コードブロックのコンポーネント
import MdImage from './components/MdImage'; // 画像のコンポーネント
import { useMdContent } from 'HUIT_SHINKAN2024/hooks/useMdContent';
import styles from './MarkdownContent.module.css';
import 'katex/dist/katex.min.css';

type MarkdownContentProps = {
  mdPath: string;
};

const MarkdownContent: FC<MarkdownContentProps> = ({ mdPath }) => {
  const markdownContent = useMdContent(mdPath ?? '');
  return (
    <div className={styles['article-wrapper']}>
      <ReactMarkdown
        remarkPlugins={[remarkDefinitionList, remarkMath, remarkGfm]}
        remarkRehypeOptions={{
          handlers: {
            ...defListHastHandlers,
          },
        }}
        rehypePlugins={[rehypeRaw, rehypeKatex, rehypeSlug]}
        components={{
          pre: (props) => <CodeBlock {...props} />,
          img: (props) => <MdImage {...props} />,
        }}
        className={styles.content}
      >
        {markdownContent}
      </ReactMarkdown>
    </div>
  );
};

export default MarkdownContent;
